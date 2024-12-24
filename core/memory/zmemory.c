#include "zmemory.h"
#include "logger.h"
#include "zmutex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct zmemory_state {
    i64 tagged_allocations[MEMORY_TAG_MAX];
    i64 allocated_memory;
    zmutex mutex;
} zmemory_state;

// default zero initialized memory (global variable)
static zmemory_state state;

bool zmemory_init() {
    state.allocated_memory = 0;
    if (!zmutex_create(&state.mutex)) {
        LOGE("zmemory_init : failed to create mutex");
        return false;
    }
    printf("zmemory_init");
    return true;
}

void zmemory_destroy() {
    zmutex_destroy(&state.mutex);
    state.allocated_memory = 0;
    printf("zmemory_destroy");
}

void* zmemory_allocate(u64 size, zmemory_tag tag) {
    void* temp = malloc(size);
    if (temp) {
        zmutex_lock(&state.mutex);
        state.allocated_memory += size;
        state.tagged_allocations[tag] += size;
        zmutex_unlock(&state.mutex);
        memset(temp, 0, size);
    }
    return temp;
}

void zmemory_free(void* block, u64 size, zmemory_tag tag) {
    if (block) {
        free(block);
        zmutex_lock(&state.mutex);
        state.allocated_memory -= size;
        state.tagged_allocations[tag] -= size;
        zmutex_unlock(&state.mutex);
    }
}

void* zmemory_set(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* zmemory_set_zero(void* block, u64 size) {
    return memset(block, 0, size);
}

void* zmemory_copy(void* dest, const void* src, u64 size) {
    return memcpy(dest, src, size);
}

void* zmemory_move(void* dest, const void* src, u64 size) {
    return memmove(dest, src, size);
}

i32 zmemory_compare(const void* block1, const void* block2, u64 size) {
    return memcmp(block1, block2, size);
}

static const char* mem_str[] = {
    "UNKNOWN              ",
    "DARRAY               ",
    "LIST                 ",
    "STACK                ",
    "QUEUE                ",
    "PRIORITY_QUEUE       ",
    "UNORDERED_MAP        ",
    "MAP                  ",
    "UNORDERED_SET        ",
    "SET                  ",
    "ALGORITHM            ",
    "LOGGER               ",
    "LINEAR_ALLOCATOR     ",
    "FREELIST_ALLOCATOR   ",
};

void zmemory_log() {

    u32 kb = 1024;
    u32 mb = 1024 * 1024;
    u32 gb = 1024 * 1024 * 1024;

    printf("\ntagged memory allocations... \n");

    for (i32 i = 0; i < MEMORY_TAG_MAX; ++i) {
        char str[3] = "XB\0";

        f32 ammount = 0.0f;

        if (state.tagged_allocations[i] >= gb) {
            str[0] = 'G';
            ammount = state.tagged_allocations[i] / (f32)(gb);
        } else if (state.tagged_allocations[i] >= mb) {
            str[0] = 'M';
            ammount = state.tagged_allocations[i] / (f32)(mb);
        } else if (state.tagged_allocations[i] >= kb) {
            str[0] = 'K';
            ammount = state.tagged_allocations[i] / (f32)(kb);
        } else {
            str[0] = 'B';
            str[1] = '\0';
            ammount = state.tagged_allocations[i];
        }

        printf("%s:%lf%s\n", mem_str[i], ammount, str);
    }

    printf("total bytes allocated = %llu\n", state.allocated_memory);
}
