#ifndef COMMON_H
#define COMMON_H

#include "defines.h"

// TODO: dont forget to change string
typedef enum memory_tag {
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_NODE,
    MEMORY_TAG_LIST,
    MEMORY_TAG_STACK,
    MEMORY_TAG_QUEUE,
    MEMORY_TAG_PRIORITY_QUEUE,
    MEMORY_TAG_UNORDERED_MAP,
    MEMORY_TAG_MAP,
    MEMORY_TAG_ALGORITHM,

    MEMORY_TAG_MAX,
} memory_tag;

typedef enum log_level {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_MAX,
} log_level;

#define LOGF(fmt, ...) log_msg(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) log_msg(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_msg(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_msg(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_msg(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) log_msg(LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

void log_msg(log_level level, const char* fmt, ...);

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

void string_format(char* buffer, u64 buffer_size, const char* fmt, ...);

f64 get_time();

void memory_state_log();

void seed_random();

i32 random(u32 range);

u32 log_buffer(void* buffer, u64 size, const char* fmt, ...);

void scan(const char* fmt, ...);

#endif