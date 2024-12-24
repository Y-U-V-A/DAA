#ifndef DEFINE__H
#define DEFINE__H

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef _Bool bool;

#define true 1
#define false 0

#define EPSILON 1.19209290e-7F

#define ABS(val) ((val) < 0 ? -(val) : (val))

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#    define ZWINDOWS

#    ifndef _WIN64
#        error "64-bit is required on Windows!"
#    endif

#elif defined(__linux__) || defined(__gnu_linux__)

#    define ZLINUX

#    if defined(__ANDROID__)
#        error "andriod not supported!"
#    endif

#else
#    error "not supported!"

#endif

#endif