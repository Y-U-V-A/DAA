#ifndef UTILS__H
#define UTILS__H

#include "defines.h"

void random_seed();

f64 random_ndc();

u32 random_int(u32 min, u32 max);

f32 random_float(f32 min, f32 max);

f64 random_double(f64 min, f64 max);

i32 string_compare(const char* str1, const char* str2);

u64 string_length(const char* str);

#ifdef ZWINDOWS

i32 string_copy(char* dest, const char* src, u64 size);

#elif defined(ZLINUX)

char* string_copy(char* dest, const char* src, u64 size);

#endif

void builtin_quick_sort(void* ptr, u64 count, u64 size,
                        int (*comp)(const void*, const void*));

#endif