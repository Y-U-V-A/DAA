#ifndef EXPECT_H
#define EXPECT_H

#include "defines.h"
#include "common.h"

#define expect_should_be(expected, actual)                                                \
    if (expected != actual) {                                                             \
        LOGE("expected %lld but got %lld , %s:%d", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                     \
    }

#define expect_should_not_be(expected, actual)                                                \
    if (expected == actual) {                                                                 \
        LOGE("not expected %lld but got %lld , %s:%d", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                         \
    }

#define expect_float_should_be(expected, actual)                                       \
    if (ABS(expected - actual) > EPSILON) {                                            \
        LOGE("expected %lf but got %lf, %s:%d", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                  \
    }

#define expect_should_be_true(actual)                                           \
    if (TRUE != actual) {                                                       \
        LOGE("expected TRUE but got %lld , %s:%d", actual, __FILE__, __LINE__); \
        return FALSE;                                                           \
    }

#define expect_should_be_false(actual)                                           \
    if (FALSE != actual) {                                                       \
        LOGE("expected FALSE but got %lld , %s:%d", actual, __FILE__, __LINE__); \
        return FALSE;                                                            \
    }

#endif