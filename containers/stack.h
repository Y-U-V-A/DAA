#ifndef STACK_H
#define STACK_H

#include "defines.h"

typedef struct stack stack;

#define stack_create(type) _stack_create(sizeof(type))

stack* _stack_create(u64 stride);

void stack_destroy(stack* stk);

void stack_push(stack* stk, void* data);

void stack_pop(stack* stk);

void* stack_top(stack* stk);

u64 stack_size(stack* stk);

#endif