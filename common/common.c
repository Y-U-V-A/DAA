#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

void log_msg(log_level level, const char* fmt, ...) {

    char buffer[1000];

    va_list args;
    va_start(args, fmt);
    u64 written = vsnprintf(buffer, 1000, fmt, args);
    va_end(args);

    buffer[written] = '\n';
    buffer[written + 1] = '\0';
    written += 1;

    static const char* log_level_str[] = {"[FATAL]: \0", "[ERROR]: \0", "[WARN]: \0", "[INFO]: \0", "[DEBUG]: \0", "[TRACE]: \0"};
    static u8 log_level_str_lengths[] = {10, 10, 9, 9, 10, 10};
    static u32 log_level_color[] = {64, 12, 14, 15, 9, 10};

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, log_level_color[level]);

    WriteConsoleA(console, log_level_str[level], (DWORD)log_level_str_lengths[level], (LPDWORD)0, 0);
    WriteConsoleA(console, buffer, (DWORD)written, (LPDWORD)0, 0);

    SetConsoleTextAttribute(console, 7);
}

typedef struct memory_state {

    u64 tagged_allocations[MEMORY_TAG_MAX];
    u64 total_allocation;

} memory_state;

// default zero initialized memory (global variable)
static memory_state state;

// memory is default set to zero
void* memory_allocate(u64 size, memory_tag tag) {

    void* block = malloc(size);
    // even thought memset takes int it gets converted to uint this is byte setting so range (0,255)
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

u64 string_length(const char* str) {
    return strlen(str);
}

void string_format(char* buffer, u64 buffer_size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, buffer_size, fmt, args);

    va_end(args);
}

f64 get_time() {

    LARGE_INTEGER frq;
    QueryPerformanceFrequency(&frq);

    LARGE_INTEGER curr;
    QueryPerformanceCounter(&curr);

    return curr.QuadPart / (f64)frq.QuadPart;
}

const char* mem_str[] = {
    "DARRAY               ",
    "NODE                 ",
    "LIST                 ",
    "STACK                ",
    "QUEUE                ",
    "PRIORITY_QUEUE       ",
    "UNORDERED_MAP        ",
    "MAP                  ",
    "ALGORITHM            ",
};

void memory_state_log() {

    u32 kb = 1024;
    u32 mb = 1024 * 1024;
    u32 gb = 1024 * 1024 * 1024;

    const u64 buffer_size = 2000;
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

        u32 written = (u32)snprintf(buffer + offset, buffer_size - offset, "%s:%lf%s\n", mem_str[i], ammount, str);
        offset += written;
    }
    LOGD("tagged memory allocations... \n%s", buffer);
}

void seed_random() {
    srand(get_time());
}

i32 random(u32 range) {
    return rand() % range;
}

u32 log_buffer(void* buffer, u64 size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    u32 written = vsnprintf(buffer, size, fmt, args);
    va_end(args);

    return written;
}

void scan(const char* fmt, ...) {

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
                scanf_s(buffer, p);
            } else if (buffer[1] == 'f') {
                float* p = va_arg(args, float*);
                scanf_s(buffer, p);
            } else if (buffer[1] == 'c') {
                char* p = va_arg(args, char*);
                scanf_s(buffer, p);
            } else if (buffer[1] == 's') {
                char* p = va_arg(args, char*);
                scanf_s(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'u') {
                long unsigned int* p = va_arg(args, long unsigned int*);
                scanf_s(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'l' && buffer[3] == 'u') {
                long long unsigned int* p = va_arg(args, long long unsigned int*);
                scanf_s(buffer, p);
            } else if (buffer[1] == 'l' && buffer[2] == 'f') {
                double* p = va_arg(args, double*);
                scanf_s(buffer, p);
            } else {
                LOGE("invalid scan format...");
            }
            // Add more types as needed...

            continue; // Move to next format specifier
        }
        ++fmt;
    }

    va_end(args);
}
