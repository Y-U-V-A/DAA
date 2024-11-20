#include "darray.h"
#include "common.h"
#include "logger.h"

void* darray_resize(void* array);

void* _darray_create(u64 length, u64 stride) {

    if (length == 0) {
        LOGW("_darray_create : length provided is zero");
    }

    if (stride == 0) {
        LOGW("_darray_create : stride provided is zero");
    }

    u64 total_size = sizeof(u64) * DARRAY_FIELD_MAX + length * stride;
    u64* temp = (u64*)memory_allocate(total_size, MEMORY_TAG_DARRAY);
    temp[DARRAY_FIELD_CAPACITY] = length;
    temp[DARRAY_FIELD_STRIDE] = stride;
    temp[DARRAY_FIELD_LENGTH] = 0;

    return (void*)(temp + DARRAY_FIELD_MAX);
}

void darray_destroy(void* array) {

    u64* temp = (u64*)array - DARRAY_FIELD_MAX;
    u64 total_size = sizeof(u64) * DARRAY_FIELD_MAX + temp[DARRAY_FIELD_CAPACITY] * temp[DARRAY_FIELD_STRIDE];

    memory_free(temp, total_size, MEMORY_TAG_DARRAY);
}

u64 _darray_get_field(void* array, darray_field field) {
    u64* temp = (u64*)array - DARRAY_FIELD_MAX;
    return temp[field];
}

void _darray_set_field(void* array, u64 value, darray_field field) {
    u64* temp = (u64*)array - DARRAY_FIELD_MAX;
    temp[field] = value;
}

void* _darray_push_back(void* array, const void* data) {

    u64 length = darray_size(array);
    u64 stride = darray_stride(array);

    if (length >= darray_capacity(array)) {
        array = darray_resize(array);
    }

    u64 addr = (u64)array;

    memory_copy((void*)(addr + length * stride), data, stride);

    darray_set_length(array, length + 1);

    return array;
}

void darray_pop_back(void* array) {

    u64 length = darray_size(array);
    if (length == 0) {
        LOGE("darray_pop_back : no elements to pop");
        return;
    }
    darray_set_length(array, length - 1);
}

void* _darray_insert(void* array, u64 index, const void* data) {

    u64 length = darray_size(array);
    u64 stride = darray_stride(array);

    if (index >= length) {
        LOGE("_darray_insert : index out of range");
    }

    if (length >= darray_capacity(array)) {
        array = darray_resize(array);
    }

    u64 addr = (u64)array;

    memory_move((void*)(addr + (index + 1) * stride),
                (void*)(addr + index * stride),
                (length - index) * stride);

    memory_copy((void*)(addr + index * stride), data, stride);

    darray_set_length(array, length + 1);

    return array;
}

void darray_remove(void* array, u64 index) {

    u64 length = darray_size(array);
    u64 stride = darray_stride(array);

    if (index >= length) {
        LOGE("_darray_insert : index out of range");
    }

    u64 addr = (u64)array;

    if (index != length - 1) {

        memory_move((void*)(addr + index * stride),
                    (void*)(addr + (index + 1) * stride),
                    (length - index - 1) * stride);
    }

    darray_set_length(array, length - 1);
}

/////////////////////////////////////////////

void* darray_resize(void* array) {

    u64 length = darray_size(array);
    u64 stride = darray_stride(array);

    void* temp = _darray_create(DARRAY_RESIZE_FACTOR * darray_capacity(array), stride);

    memory_copy(temp, array, length * stride);
    darray_set_length(temp, length);

    darray_destroy(array);
    return temp;
}