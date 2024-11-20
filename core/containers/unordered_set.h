#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

#include "defines.h"

#define UNORDERED_SET_DEFAULT_SIZE 20
#define UNORDERED_SET_RESIZE_FACTOR 2
#define UNORDERED_SET_LOAD_FACTOR 0.7

typedef struct unordered_set unordered_set;

typedef u64 (*PFN_unordered_set_hash)(const void* data, u64 stride);

// pass zero for default hash;
#define unordered_set_create(type, PFN_unordered_set_hash) \
    _unordered_set_create(UNORDERED_SET_DEFAULT_SIZE, sizeof(type), PFN_unordered_set_hash)

#define unordered_set_insert(un_set, data) un_set = _unordered_set_insert(un_set, data)

unordered_set* _unordered_set_create(u64 size, u64 data_stride, PFN_unordered_set_hash hash_func);

void unordered_set_destroy(unordered_set* un_set);

unordered_set* _unordered_set_insert(unordered_set* un_set, const void* data);

void unordered_set_remove(unordered_set* un_set, const void* data);

unordered_set* unordered_set_resize(unordered_set* un_set);

u64 unordered_set_size(unordered_set* un_set);

u64 unordered_set_capacity(unordered_set* un_set);

bool unordered_set_contains(unordered_set* un_set, const void* data);

#endif