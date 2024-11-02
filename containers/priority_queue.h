#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "defines.h"

typedef struct priority_queue priority_queue;

typedef b8 (*PFN_priority_cmp)(void* parent, void* child);

#define priority_queue_create(type) _priority_queue_create(sizeof(type), PFN_priority_cmp);

priority_queue* _priority_queue_create(u64 stride, PFN_priority_cmp cmp_func);

void priority_queue_destroy(priority_queue* prio_que);

void priority_queue_push(priority_queue* prio_que, void* data);

void priority_queue_pop(priority_queue* prio_que);

void* priority_queue_top(priority_queue* prio_que);

u64 priority_queue_size(priority_queue* prio_que);

#endif