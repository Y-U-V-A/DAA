#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef bool b8;

#define EPSILON __FLT_EPSILON__
#define CLAMP(val, min, max) ((val >= max ? max : (val <= min ? min : val)))
#define ABS(val) ((val < 0) ? -val : val)

#define _ASSERT_GLUE(a, b) a##b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)
#define STATIC_ASSERT(exp) enum {                       \
    ASSERT_GLUE(assert_, __LINE__) = 1 / (int)(!!(exp)) \
};

STATIC_ASSERT(sizeof(u8) == 1)
STATIC_ASSERT(sizeof(u16) == 2)
STATIC_ASSERT(sizeof(u32) == 4)
STATIC_ASSERT(sizeof(u64) == 8)

STATIC_ASSERT(sizeof(i8) == 1)
STATIC_ASSERT(sizeof(i16) == 2)
STATIC_ASSERT(sizeof(i32) == 4)
STATIC_ASSERT(sizeof(i64) == 8)

STATIC_ASSERT(sizeof(f32) == 4)
STATIC_ASSERT(sizeof(f64) == 8)

#endif