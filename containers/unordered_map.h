#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "defines.h"

#define UNORDERED_MAP_DEFAULT_SIZE 20
#define UNORDERED_MAP_RESIZE_FACTOR 2
#define UNORDERED_MAP_LOAD_FACTOR 0.7

typedef struct unordered_map unordered_map;

typedef u64 (*PFN_unordered_map_hash)(const void* key, u64 key_stride);

#define unordered_map_create(key_type, data_type, PFN_unordered_map_hash) \
    _unordered_map_create(UNORDERED_MAP_DEFAULT_SIZE, sizeof(key_type), sizeof(data_type), PFN_unordered_map_hash)

unordered_map* _unordered_map_create(u64 size, u64 key_stride, u64 data_stride, PFN_unordered_map_hash hash_func);

void unordered_map_destroy(unordered_map* un_map);

void unordered_map_insert(unordered_map* un_map, const void* key, const void* data);

void* unordered_map_data(unordered_map* un_map, const void* key);

void unordered_map_remove(unordered_map* un_map, const void* key);

void unordered_map_resize(unordered_map* un_map);

u64 unordered_map_size(unordered_map* un_map);

u64 unordered_map_capacity(unordered_map* un_map);

#endif