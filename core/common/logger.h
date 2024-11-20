#ifndef LOGGER__H
#define LOGGER__H

#include "defines.h"

typedef enum log_level {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,

    LOG_LEVEL_MAX,
} log_level;

#define LOGE(fmt, ...) log_output(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_output(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_output(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_output(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) log_output(LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

void log_output(log_level level, const char* fmt, ...);

u32 log_buffer(char* buffer, u64 buffer_size, const char* fmt, ...);

void scan(const char* fmt, ...);

void logger_init(u64 buffer_size);

void logger_shutdown();

#endif