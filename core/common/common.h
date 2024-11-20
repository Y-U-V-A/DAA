#ifndef COMMON_H
#define COMMON_H

#include "defines.h"

// TODO: dont forget to change string
typedef enum memory_tag {
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

    MEMORY_TAG_MAX,
} memory_tag;

// memory is default set to zero
void* memory_allocate(u64 size, memory_tag tag);

void memory_free(void* block, u64 size, memory_tag tag);

void* memory_set(void* block, i32 value, u64 size);

void* memory_set_zero(void* block, u64 size);

void* memory_copy(void* dest, const void* src, u64 size);

void* memory_move(void* dest, const void* src, u64 size);

i32 memory_compare(const void* block1, const void* block2, u64 size);

i32 string_compare(const char* str1, const char* str2);

u64 string_length(const char* str);

#ifdef ZWINDOWS

i32 string_copy(char* dest, const char* src, u64 size);

#elif defined(ZLINUX)

char* string_copy(char* dest, const char* src, u64 size);

#endif

void memory_state_log();

void seed_random();

i32 generate_random(u32 max);

#endif