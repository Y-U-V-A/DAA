#ifndef LIST_H
#define LIST_H

#include "defines.h"

typedef struct list list;

typedef b8 (*PFN_list_cmp)(const void* data_1, const void* data_2);

#define list_create(type) _list_create(sizeof(type))

list* _list_create(u64 stride);

void list_destroy(list* lst);

void list_push_back(list* lst, const void* data);

void list_pop_back(list* lst);

void list_push_front(list* lst, const void* data);

void list_pop_front(list* lst);

void list_insert(list* lst, u64 index, const void* data);

void list_remove(list* lst, u64 index);

void list_clear(list* lst);

u64 list_size(list* lst);

void* list_data(list* lst, u64 index);

void list_sort(list* lst, PFN_list_cmp cmp_func);

void list_merge(list* lst_final, list* lst, PFN_list_cmp cmp_func);

void list_reverse(list* lst);

void list_unique(list* sorted_lst, PFN_list_cmp cmp_func);

#endif