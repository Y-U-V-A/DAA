#ifndef DARRAY_H
#define DARRAY_H

#include "defines.h"

// format
//{u64,u64,u64,data};
typedef enum darray_field {
    DARRAY_FIELD_CAPACITY,
    DARRAY_FIELD_STRIDE,
    DARRAY_FIELD_LENGTH,
    DARRAY_FIELD_MAX,
} darray_field;

#define DARRAY_DEFAULT_LENGTH 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(type) (type*)_darray_create(DARRAY_DEFAULT_LENGTH, sizeof(type))
#define darray_reserve(length, type) (type*)_darray_create(length, sizeof(type))

#define darray_size(darray) _darray_get_field(darray, DARRAY_FIELD_LENGTH)
#define darray_capacity(darray) _darray_get_field(darray, DARRAY_FIELD_CAPACITY)
#define darray_stride(darray) _darray_get_field(darray, DARRAY_FIELD_STRIDE)

#define darray_clear(darray) _darray_set_field(darray, 0, DARRAY_FIELD_LENGTH)
#define darray_set_length(darray, value) _darray_set_field(darray, value, DARRAY_FIELD_LENGTH)

#define darray_push_back(darray, data)                             \
    {                                                              \
        typeof(data) xyz__ = data;                                 \
        darray = (typeof(data)*)_darray_push_back(darray, &xyz__); \
    }

#define darray_insert(darray, index, data)                             \
    {                                                                  \
        typeof(data) xyz__ = data;                                     \
        darray = (typeof(data)*)_darray_insert(darray, index, &xyz__); \
    }

void* _darray_create(u64 length, u64 stride);

void darray_destroy(void* darray);

u64 _darray_get_field(void* darray, darray_field field);

void _darray_set_field(void* darray, u64 value, darray_field field);

void* _darray_push_back(void* darray, const void* data);

void darray_pop_back(void* darray);

void* _darray_insert(void* darray, u64 index, const void* data);

void darray_remove(void* darray, u64 index);

#endif