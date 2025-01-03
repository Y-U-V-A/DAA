#ifndef PRIORITY_QUEUE__H
#define PRIORITY_QUEUE__H

#include "defines.h"

typedef struct priority_queue priority_queue;

typedef bool (*PFN_priority_cmp)(const void* parent, const void* child);

#define priority_queue_create(type, PFN_priority_cmp) _priority_queue_create(sizeof(type), PFN_priority_cmp)

priority_queue* _priority_queue_create(u64 stride, PFN_priority_cmp cmp_func);

void priority_queue_destroy(priority_queue* prio_que);

void priority_queue_push(priority_queue* prio_que, const void* data);

void priority_queue_pop(priority_queue* prio_que);

void* priority_queue_top(priority_queue* prio_que);

u64 priority_queue_length(priority_queue* prio_que);

#endif