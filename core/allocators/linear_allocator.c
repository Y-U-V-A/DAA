#include "linear_allocator.h"
#include "zmemory.h"
#include "logger.h"
#include "zmutex.h"

#define ALIGN_UP(val, alignment) (((val) + ((alignment) - 1)) & (~((alignment) - 1)))
#define IS_POWER_OF_TWO(val) (((val) != 0) && (((val) & ((val) - 1)) == 0))

typedef struct linear_allocator {
    void* block;
    u64 size;
    u64 used;
    zmutex mutex;
} linear_allocator;

linear_allocator* linear_allocator_create(u64 size) {
    if (size == 0) {
        LOGE("linear_allocator_create: invalid parameters");
        return 0;
    }

    linear_allocator* allocator = zmemory_allocate(sizeof(linear_allocator), MEMORY_LINEAR_ALLOCATOR);
    allocator->size = size;
    allocator->used = 0;
    allocator->block = zmemory_allocate(size, MEMORY_LINEAR_ALLOCATOR);
    if (allocator->block == 0) {
        LOGE("linear_allocator_create: failed to allocate size = %llu", size);
        return 0;
    }
    if (!zmutex_create(&allocator->mutex)) {
        LOGE("linear_allocator_create: failed to create mutex");
        return 0;
    }

    LOGT("linear_allocator_create");

    return allocator;
}

void linear_allocator_destroy(linear_allocator* allocator) {
    if (allocator == 0) {
        LOGE("linear_allocator_destroy: invalid parameters");
        return;
    }

    zmutex_destroy(&allocator->mutex);
    zmemory_free(allocator->block, allocator->size, MEMORY_LINEAR_ALLOCATOR);
    zmemory_free(allocator, sizeof(linear_allocator), MEMORY_LINEAR_ALLOCATOR);

    LOGT("linear_allocator_destroy");
}

void* linear_allocator_allocate_aligned(linear_allocator* allocator, u64 size, linear_allocator_memory_alignment memory_alignment) {

    if (size == 0 || allocator == 0 || IS_POWER_OF_TWO(memory_alignment) == 0 || size > allocator->size) {
        LOGE("linear_allocator_allocate: invalid params ");
        return 0;
    }
    if (allocator->size - allocator->used < size) {
        LOGW("linear_allocator_allocate_aligned : no free space");
        return 0;
    }

    zmutex_lock(&allocator->mutex);

    u64 alignment = (u64)memory_alignment;
    u64 curr_addr = (u64)allocator->block + allocator->used;
    u64 aligned_addr = ALIGN_UP(curr_addr, alignment);
    u64 padding = (aligned_addr - curr_addr);

    if ((allocator->used + padding + size) > allocator->size) {
        LOGW("linear_allocator_allocate: no free space (requested %llu,padding %llu,alignment %llu,available %llu)",
             size, padding, alignment, allocator->size - allocator->used);
        zmutex_unlock(&allocator->mutex);
        return 0;
    }

    allocator->used += padding;
    allocator->used += size;

    zmutex_unlock(&allocator->mutex);

    return (void*)aligned_addr;
}

void linear_allocator_reset(linear_allocator* allocator) {

    if (allocator == 0) {
        LOGE("linear_allocator_destroy: invalid parameters");
        return;
    }
    zmutex_lock(&allocator->mutex);
    allocator->used = 0;
    zmutex_unlock(&allocator->mutex);

    LOGT("linear_allocator_reset");
}

u64 linear_allocator_used_memory(linear_allocator* allocator) {
    return allocator->used;
}

u64 linear_allocator_unused_memory(linear_allocator* allocator) {
    return allocator->size - allocator->used;
}