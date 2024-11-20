#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "platform.h"

typedef struct memory_state {

    u64 tagged_allocations[MEMORY_TAG_MAX];
    u64 total_allocation;

} memory_state;

// default zero initialized memory (global variable)
static memory_state state;

// memory is default set to zero
void* memory_allocate(u64 size, memory_tag tag) {

    void* block = malloc(size);
    // even thought memset takes int it gets converted to uint this is byte setting so max (0,255)
    memset(block, 0, size);

    state.total_allocation += size;
    state.tagged_allocations[tag] += size;

    return block;
}

void memory_free(void* block, u64 size, memory_tag tag) {

    free(block);

    state.tagged_allocations[tag] -= size;
    state.total_allocation -= size;
}

void* memory_set(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* memory_set_zero(void* block, u64 size) {
    return memset(block, 0, size);
}

void* memory_copy(void* dest, const void* src, u64 size) {
    return memcpy(dest, src, size);
}

void* memory_move(void* dest, const void* src, u64 size) {
    return memmove(dest, src, size);
}

i32 memory_compare(const void* block1, const void* block2, u64 size) {
    return memcmp(block1, block2, size);
}

i32 string_compare(const char* str1, const char* str2) {
    return strcmp(str1, str2);
}

#ifdef ZWINDOWS
i32 string_copy(char* dest, const char* src, u64 size) {
    return strcpy_s(dest, size, src);
}

#elif defined(ZLINUX)

char* string_copy(char* dest, const char* src, u64 size) {
    return strcpy(dest, src);
}

#endif

u64 string_length(const char* str) {
    return strlen(str);
}

const char* mem_str[] = {
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
};

void memory_state_log() {

    u32 kb = 1024;
    u32 mb = 1024 * 1024;
    u32 gb = 1024 * 1024 * 1024;

    static const int buffer_size = 1024;
    char buffer[buffer_size] = {0};

    u32 offset = 0;

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

        offset += log_buffer(buffer + offset, buffer_size - offset, "%s:%lf%s\n", mem_str[i], ammount, str);
    }

    LOGD("tagged memory allocations... \n%s\ntotal bytes allocated = %lu", buffer, state.total_allocation);
}

void seed_random() {
    srand(platform_time());
}

i32 generate_random(u32 max) {
    if (max == 0) {
        max = RAND_MAX;
    }
    return rand() % max;
}
