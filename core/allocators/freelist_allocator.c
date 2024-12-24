#include "freelist_allocator.h"
#include "logger.h"
#include "zmemory.h"
#include "zmutex.h"
#include "unordered_set.h"

////////////////////////////////////////////////////////////////////////
//   ______                               __  __              __      //
//  /      \                             /  |/  |            /  |     //
// /$$$$$$  |______    ______    ______  $$ |$$/   _______  _$$ |_    //
// $$ |_ $$//      \  /      \  /      \ $$ |/  | /       |/ $$   |   //
// $$   |  /$$$$$$  |/$$$$$$  |/$$$$$$  |$$ |$$ |/$$$$$$$/ $$$$$$/    //
// $$$$/   $$ |  $$/ $$    $$ |$$    $$ |$$ |$$ |$$      \   $$ | __  //
// $$ |    $$ |      $$$$$$$$/ $$$$$$$$/ $$ |$$ | $$$$$$  |  $$ |/  | //
// $$ |    $$ |      $$       |$$       |$$ |$$ |/     $$/   $$  $$/  //
// $$/     $$/        $$$$$$$/  $$$$$$$/ $$/ $$/ $$$$$$$/     $$$$/   //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#define ALIGN_UP(val, alignment) (((val) + ((alignment) - 1)) & (~((alignment) - 1)))
#define IS_POWER_OF_TWO(val) (((val) != 0) && (((val) & ((val) - 1)) == 0))
#define FREELIST_HEADER_SIZE sizeof(freelist_header)

typedef struct freelist_header {
    u64 unique; // 0xF7B3D591E6A4C208
    u64 size;
    struct freelist_header* next;
    struct freelist_header* prev;
} freelist_header;

typedef struct freelist_allocator {
    void* block;
    u64 size;
    u64 used;
    freelist_header* head;
    zmutex mutex;
} freelist_allocator;

freelist_header* get_best_fit_block(freelist_header* node, u64 size);
void freelist_coalescing(freelist_allocator* allocator);

freelist_allocator* freelist_allocator_create(u64 size) {
    if (size == 0 || IS_POWER_OF_TWO(size) == 0) {
        LOGE("freelist_allocator_create : invalid params");
        return 0;
    }

    freelist_allocator* allocator = zmemory_allocate(sizeof(freelist_allocator), MEMORY_FREELIST_ALLOCATOR);
    allocator->block = zmemory_allocate(size, MEMORY_FREELIST_ALLOCATOR);
    if (allocator->block == 0) {
        LOGE("freelist_allocator_create:failed to allocate memory");
        zmemory_free(allocator, sizeof(freelist_allocator), MEMORY_FREELIST_ALLOCATOR);
        return 0;
    }
    allocator->size = size;
    allocator->used = 0;
    allocator->head = allocator->block;
    allocator->head->unique = 0;
    allocator->head->size = size;
    allocator->head->next = 0;
    allocator->head->prev = 0;
    if (!zmutex_create(&allocator->mutex)) {
        LOGE("freelist_allocator_create : failed to create mutex");
        zmemory_free(allocator->block, size, MEMORY_FREELIST_ALLOCATOR);
        zmemory_free(allocator, sizeof(freelist_allocator), MEMORY_FREELIST_ALLOCATOR);
        return 0;
    }
    LOGT("freelist_allocator_create");
    return allocator;
}

void freelist_allocator_destroy(freelist_allocator* allocator) {
    if (allocator == 0) {
        LOGE("freelist_allocator_destroy : invalid params");
        return;
    }
    zmutex_destroy(&allocator->mutex);
    zmemory_free(allocator->block, allocator->size, MEMORY_FREELIST_ALLOCATOR);
    zmemory_free(allocator, sizeof(freelist_allocator), MEMORY_FREELIST_ALLOCATOR);

    LOGT("freelist_allocator_destroy");
}

// default 8 byte alignment;
void* freelist_allocator_allocate(freelist_allocator* allocator, u64 size) {
    if (allocator == 0 || size == 0 || size >= allocator->size) {
        LOGE("freelist_allocator_allocate : invalid params");
        return 0;
    }

    if (allocator->head == 0 || (allocator->size - allocator->used) <= size) {
        LOGW("freelist_allocator_allocate : no free space");
        return 0;
    }

    zmutex_lock(&allocator->mutex);

    freelist_header* block = get_best_fit_block(allocator->head, size + FREELIST_HEADER_SIZE);
    if (block == 0) {
        freelist_coalescing(allocator);

        block = get_best_fit_block(allocator->head, size + FREELIST_HEADER_SIZE);
        if (block == 0) {
            LOGW("freelist_allocator_allocate: no free space");
            zmutex_unlock(&allocator->mutex);
            return 0;
        }
    }
    u64 start_addr = (u64)block;
    u64 end_addr = (u64)block + FREELIST_HEADER_SIZE + size;
    u64 aligned_end_addr = ALIGN_UP(end_addr, 8);
    u64 used_size = aligned_end_addr - start_addr;
    u64 unused_size = block->size - used_size;
    // only then split the block
    if (unused_size > FREELIST_HEADER_SIZE) {
        freelist_header* split = (freelist_header*)aligned_end_addr;
        split->prev = block->prev;
        split->next = block->next;
        split->size = unused_size;
        split->unique = 0;

        if (block->prev == 0) {
            allocator->head = split;
            if (block->next) {
                block->next->prev = split;
            }
        } else {
            block->prev->next = split;
            if (block->next) {
                block->next->prev = split;
            }
        }
        block->size = used_size;
    } else {
        if (block->prev == 0) {
            allocator->head = block->next;
            if (block->next) {
                block->next->prev = 0;
            }
        } else {
            block->prev->next = block->next;
            if (block->next) {
                block->next->prev = block->prev;
            }
        }
    }

    block->next = 0;
    block->prev = 0;
    block->unique = 0xF7B3D591E6A4C208;
    allocator->used += used_size;

    zmutex_unlock(&allocator->mutex);

    return (u8*)block + FREELIST_HEADER_SIZE;
}

void freelist_allocator_free(freelist_allocator* allocator, void* block) {
    if (allocator == 0 || block == 0) {
        LOGE("freelist_allocator_free : invalid params");
        return;
    }
    zmutex_lock(&allocator->mutex);

    freelist_header* remove_block = (freelist_header*)((u8*)block - FREELIST_HEADER_SIZE);
    if (((u64)remove_block < (u64)allocator->block) ||
        ((u64)remove_block >= ((u64)allocator->block + allocator->size)) ||
        remove_block->unique != 0xF7B3D591E6A4C208) {
        LOGE("freelist_allocator_free : invalid block addr");
        return;
    }

    allocator->used -= remove_block->size;

    remove_block->unique = 0;

    freelist_header* node = allocator->head;
    freelist_header* prev = 0;
    u64 next_block;
    u64 prev_block;

    while (node) {
        next_block = (u64)node + node->size;
        prev_block = (u64)node - remove_block->size;
        if (next_block == (u64)remove_block) {
            node->size += remove_block->size;
            zmutex_unlock(&allocator->mutex);
            return;
        } else if (prev_block == (u64)remove_block) {
            remove_block->next = node->next;
            remove_block->prev = node->prev;
            remove_block->size += node->size;

            if (node->prev) {
                node->prev->next = remove_block;
            } else {
                allocator->head = remove_block;
            }
            if (node->next) {
                node->next->prev = remove_block;
            }
            node->unique = 0;
            zmutex_unlock(&allocator->mutex);
            return;
        }
        prev = node;
        node = node->next;
    }

    prev->next = remove_block;
    remove_block->next = 0;
    remove_block->prev = prev;

    zmutex_unlock(&allocator->mutex);
}

void freelist_allocator_reset(freelist_allocator* allocator) {
    if (allocator == 0) {
        LOGE("freelist_allocator_reset : invalid params");
        return;
    }
    zmutex_lock(&allocator->mutex);

    allocator->head = allocator->block;
    allocator->head->next = 0;
    allocator->head->prev = 0;
    allocator->head->size = allocator->size;
    allocator->head->unique = 0;
    allocator->used = 0;

    zmutex_unlock(&allocator->mutex);
}

u64 freelist_allocator_used_memory(freelist_allocator* allocator) {
    return allocator->used;
}

u64 freelist_allocator_unused_memory(freelist_allocator* allocator) {
    return allocator->size - allocator->used;
}

u64 freelist_allocator_header_size() {
    return FREELIST_HEADER_SIZE;
}

//////////////////////////////////////////////////////////////////////
//  __                  __                                          //
// /  |                /  |                                         //
// $$ |____    ______  $$ |  ______    ______    ______    _______  //
// $$      \  /      \ $$ | /      \  /      \  /      \  /       | //
// $$$$$$$  |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$/  //
// $$ |  $$ |$$    $$ |$$ |$$ |  $$ |$$    $$ |$$ |  $$/ $$      \  //
// $$ |  $$ |$$$$$$$$/ $$ |$$ |__$$ |$$$$$$$$/ $$ |       $$$$$$  | //
// $$ |  $$ |$$       |$$ |$$    $$/ $$       |$$ |      /     $$/  //
// $$/   $$/  $$$$$$$/ $$/ $$$$$$$/   $$$$$$$/ $$/       $$$$$$$/   //
//                         $$ |                                     //
//                         $$ |                                     //
//                         $$/                                      //
//                                                                  //
//////////////////////////////////////////////////////////////////////

freelist_header* get_best_fit_block(freelist_header* node, u64 size) {
    freelist_header* best = 0;
    u64 min_extra = 100000000;

    while (node) {
        if (node->size >= size && (node->size - size) < min_extra) {
            min_extra = node->size - size;
            best = node;
        }
        node = node->next;
    }
    return best;
}

void freelist_coalescing(freelist_allocator* allocator) {
    LOGT("freelist_allocator : undergoing coalescing ");

    freelist_header* node = allocator->head;
    unordered_set* set = unordered_set_create(freelist_header*, 0);
    while (node) {
        unordered_set_insert(set, node);
        node = node->next;
    }

    node = allocator->head;
    while (node) {
        freelist_header* next_node = (freelist_header*)((u8*)node + node->size);
        while (unordered_set_contains(set, next_node)) {
            node->size += next_node->size;
            if (next_node->prev) {
                next_node->prev->next = next_node->next;
            } else {
                if (next_node->next)
                    allocator->head = next_node->next;
            }
            if (next_node->next) {
                next_node->next->prev = next_node->prev;
            }

            next_node->prev = 0;
            next_node->next = 0;
            next_node->size = 0;
            next_node->unique = 0;
            next_node = (freelist_header*)((u8*)node + node->size);
        }
        node = node->next;
    }

    unordered_set_destroy(set);
    LOGT("freelist_allocator : coalescing completed ");
}