#ifndef ZMEMORY__H
#define ZMEMORY__H

#include "defines.h"

// TODO: dont forget to change string
typedef enum zmemory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_LIST,
    MEMORY_TAG_STACK,
    MEMORY_TAG_QUEUE,
    MEMORY_TAG_PRIORITY_QUEUE,
    MEMORY_TAG_UNORDERED_MAP,
    MEMORY_TAG_MAP,
    MEMORY_TAG_UNORDERED_SET,
    MEMORY_TAG_SET,
    MEMORY_TAG_ALGORITHM,
    MEMORY_TAG_LOGGER,
    MEMORY_LINEAR_ALLOCATOR,
    MEMORY_FREELIST_ALLOCATOR,

    MEMORY_TAG_MAX,
} zmemory_tag;

bool zmemory_init();

void zmemory_destroy();

// memory is default set to zero
void* zmemory_allocate(u64 size, zmemory_tag tag);

void zmemory_free(void* block, u64 size, zmemory_tag tag);

void* zmemory_set(void* block, i32 value, u64 size);

void* zmemory_set_zero(void* block, u64 size);

void* zmemory_copy(void* dest, const void* src, u64 size);

void* zmemory_move(void* dest, const void* src, u64 size);

i32 zmemory_compare(const void* block1, const void* block2, u64 size);

void zmemory_log();

#endif