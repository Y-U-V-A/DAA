#ifndef SET_H
#define SET_H

#include "defines.h"

typedef struct set set;

typedef struct set_node {
    void* data;
    struct set_node* left;
    struct set_node* right;
    struct set_node* parent;
    char is_red;
} set_node;

typedef b8 (*PFN_set_cmp)(const void* root_key, const void* node_key);

#define set_create(type, PFN_set_cmp) _set_create(sizeof(type), PFN_set_cmp)

set* _set_create(u64 data_stride, PFN_set_cmp cmp_func);

void set_destroy(set* st);

void set_insert(set* st, const void* data);

void set_remove(set* st, const void* data);

u64 set_size(set* st);

set_node* set_begin(set* st);

set_node* set_next(set* st, set_node* node);

set_node* set_end(set* st);

bool set_contains(set* st, const void* data);

#endif