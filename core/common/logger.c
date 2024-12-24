#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include "assert.h"
#include "platform.h"
#include "zmemory.h"
#include "zmutex.h"

typedef struct logger_state {
    void* buffer;
    u64 size;
    zmutex mutex;
} logger_state;

static logger_state state;

void logger_init(u64 buffer_size) {
    state.buffer = zmemory_allocate(buffer_size, MEMORY_TAG_LOGGER);
    state.size = buffer_size;
    zmutex_create(&state.mutex);
}

void logger_shutdown() {
    zmutex_destroy(&state.mutex);
    zmemory_free(state.buffer, state.size, MEMORY_TAG_LOGGER);
    state.buffer = 0;
    state.size = 0;
}

void log_output(log_level level, const char* fmt, ...) {
    if (state.size == 0) {
        DEBUG_BREAK;
        return;
    }

    static const char* log_level_str[5] = {
        "[ERROR]: ",
        "[WARN]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: ",
    };
    static const u8 log_level_str_len[5] = {10, 9, 9, 10, 10};

    zmutex_lock(&state.mutex);
    va_list args;
    va_start(args, fmt);
    i32 written = vsnprintf(state.buffer, state.size, fmt, args);
    va_end(args);

    char* temp = state.buffer;
    temp[written++] = '\n';
    temp[written++] = '\0';

    platform_console_write((i32)level, log_level_str[level], log_level_str_len[level]);
    platform_console_write((i32)level, state.buffer, written);

    zmutex_unlock(&state.mutex);
}

void logz_output(log_level level, const char* fmt, ...) {
    if (state.size == 0) {
        DEBUG_BREAK;
        return;
    }

    zmutex_lock(&state.mutex);
    va_list args;
    va_start(args, fmt);
    i32 written = vsnprintf(state.buffer, state.size, fmt, args);
    va_end(args);

    char* temp = state.buffer;
    temp[written++] = '\0';

    platform_console_write((i32)level, state.buffer, written);

    zmutex_unlock(&state.mutex);
}

u32 log_buffer(char* buffer, u64 buffer_size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    i32 written = vsnprintf(buffer, buffer_size, fmt, args);

    va_end(args);

    return (u32)written;
}

void log_assert(const char* exp, const char* file, i32 line) {
    log_output(LOG_LEVEL_ERROR, "assert %s , %s:%d", exp, file, line);
}

#ifdef ZWINDOWS
#    define SCN scanf_s
#elif defined(ZLINUX)
#    define SCN scanf
#endif

// TODO:fix
void input(const char* fmt, ...) {

    va_list args;
    va_start(args, fmt);

    char buffer[10];

    while (*fmt != '\0') {

        if (*fmt == '%') {
            ++fmt;

            int i = 0;
            buffer[i++] = '%'; // Add '%'
            while (*fmt == 'l' || *fmt == 'u' || *fmt == 'i' || *fmt == 'd' || *fmt == 'f' || *fmt == 'c' || *fmt == 's') {

                buffer[i++] = *fmt++;
            }

            buffer[i] = '\0'; // Null-terminate the format specifier

            printf("%s = ", buffer);

            // Handle different format specifiers dynamically
            if (buffer[1] == 'd' || buffer[1] == 'i') {
                int* p = va_arg(args, int*);
                SCN(buffer, p);
            } else if (buffer[1] == 'f') {
                float* p = va_arg(args, float*);
                SCN(buffer, p);
            } else if (buffer[1] == 'c') {
                char* p = va_arg(args, char*);
                SCN(buffer, p);
            } else if (buffer[1] == 'u') {
                unsigned int* p = va_arg(args, unsigned int*);
                SCN(buffer, p);
            } else if (buffer[1] == 's') {
                char* p = va_arg(args, char*);
                SCN(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'u') {
                long unsigned int* p = va_arg(args, long unsigned int*);
                SCN(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'l' && buffer[3] == 'u') {
                long long unsigned int* p = va_arg(args, long long unsigned int*);
                SCN(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'f') {
                double* p = va_arg(args, double*);
                SCN(buffer, p);
            } else {
                LOGE("invalid input format...");
            }
            // Add more types as needed...

            continue; // Move to next format specifier
        }
        ++fmt;
    }

    va_end(args);
}