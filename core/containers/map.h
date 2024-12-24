#ifndef MAP__H
#define MAP__H

#include "defines.h"

typedef struct map map;

typedef struct map_node {
    void* key;
    void* data;
    struct map_node* left;
    struct map_node* right;
    struct map_node* parent;
    char is_red;
} map_node;

typedef bool (*PFN_map_cmp)(const void* root_key, const void* node_key);

#define map_create(key_type, data_type, PFN_map_cmp) _map_create(sizeof(key_type), sizeof(data_type), PFN_map_cmp)

map* _map_create(u64 key_stride, u64 data_stride, PFN_map_cmp cmp_func);

void map_destroy(map* mp);

void map_insert(map* mp, const void* key, const void* data);

void map_remove(map* mp, const void* key);

u64 map_length(map* mp);

void* map_data(map* mp, const void* key);

map_node* map_begin(map* mp);

map_node* map_next(map* mp, map_node* node);

map_node* map_end(map* mp);

bool map_contains(map* mp, const void* key);

#endif