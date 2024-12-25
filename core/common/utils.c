#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include "platform.h"

void random_seed() {
    srand(platform_time());
}

f64 random_ndc() {
    return rand() / (f64)RAND_MAX;
}

u32 random_int(u32 min, u32 max) {
    if (min >= max)
        return min;

    // Add 1 to max to make the range inclusive
    u32 range = max - min + 1;
    // Use modulo to avoid floating-point conversion
    return min + (rand() % range);
}

f32 random_float(f32 min, f32 max) {
    return (f32)(random_ndc() * (max - min) + min);
}

f64 random_double(f64 min, f64 max) {
    return (f64)(random_ndc() * (max - min) + min);
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

void builtin_quick_sort(void* ptr, u64 count, u64 size,
                        int (*comp)(const void*, const void*)) {
    qsort(ptr, count, size, comp);
}