#ifndef QUEUE_H
#define QUEUE_H

#include "defines.h"

typedef struct queue queue;

#define queue_create(type) _queue_create(sizeof(type))

queue* _queue_create(u64 stride);

void queue_destroy(queue* que);

void queue_push(queue* que, const void* data);

void queue_pop(queue* que);

void* queue_front(queue* que);

void* queue_back(queue* que);

u64 queue_size(queue* que);

#endif