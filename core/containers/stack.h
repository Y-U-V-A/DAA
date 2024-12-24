#ifndef STACK__H
#define STACK__H

#include "defines.h"

typedef struct stack stack;

#define stack_create(type) _stack_create(sizeof(type))

stack* _stack_create(u64 stride);

void stack_destroy(stack* stk);

void stack_push(stack* stk, const void* data);

void stack_pop(stack* stk);

void* stack_top(stack* stk);

u64 stack_length(stack* stk);

#endif