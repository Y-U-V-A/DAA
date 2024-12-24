#include "stack.h"
#include "zmemory.h"
#include "logger.h"

////////////////////////////////////////////////////////
//              __                          __        //
//             /  |                        /  |       //
//   _______  _$$ |_     ______    _______ $$ |   __  //
//  /       |/ $$   |   /      \  /       |$$ |  /  | //
// /$$$$$$$/ $$$$$$/    $$$$$$  |/$$$$$$$/ $$ |_/$$/  //
// $$      \   $$ | __  /    $$ |$$ |      $$   $$<   //
//  $$$$$$  |  $$ |/  |/$$$$$$$ |$$ \_____ $$$$$$  \  //
// /     $$/   $$  $$/ $$    $$ |$$       |$$ | $$  | //
// $$$$$$$/     $$$$/   $$$$$$$/  $$$$$$$/ $$/   $$/  //
//                                                    //
////////////////////////////////////////////////////////

typedef struct stack_node {

    void* data;
    struct stack_node* next;

} stack_node;

stack_node* create_stack_node(const void* data, u64 stride);

void destroy_stack_node(stack_node* node, u64 stride);

typedef struct stack {

    stack_node* top;
    u64 size;
    u64 stride;
} stack;

stack* _stack_create(u64 stride) {
    stack* temp = (stack*)zmemory_allocate(sizeof(stack), MEMORY_TAG_STACK);
    temp->size = 0;
    temp->top = 0;
    temp->stride = stride;

    return temp;
}

void stack_destroy(stack* stk) {
    if (stk->top) {
        destroy_stack_node(stk->top, stk->stride);
    }
    zmemory_free(stk, sizeof(stack), MEMORY_TAG_STACK);
}

void stack_push(stack* stk, const void* data) {
    stack_node* node = create_stack_node(data, stk->stride);

    if (stk->top == 0) {
        stk->top = node;
    } else {
        stack_node* temp = stk->top;
        stk->top = node;
        node->next = temp;
    }
    stk->size += 1;
}

void stack_pop(stack* stk) {

    if (stk->top == 0) {
        LOGW("stack_pop : stack has zero nodes");
        return;
    }

    stack_node* temp = stk->top;
    stk->top = temp->next;
    temp->next = 0;

    destroy_stack_node(temp, stk->stride);

    stk->size -= 1;
}

void* stack_top(stack* stk) {
    if (stk->top == 0) {
        LOGW("stack_top : stack has zero nodes");
        return 0;
    }

    return stk->top->data;
}

u64 stack_length(stack* stk) {
    return stk->size;
}

//////////////////////////////////////////////////////////////////////
//  __                  __                                          //
// /  |                /  |                                         //
// $$ |____    ______  $$ |  ______    ______    ______    _______  //
// $$      \  /      \ $$ | /      \  /      \  /      \  /       | //
// $$$$$$$  |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$/  //
// $$ |  $$ |$$    $$ |$$ |$$ |  $$ |$$    $$ |$$ |  $$/ $$      \  //
// $$ |  $$ |$$$$$$$$/ $$ |$$ |__$$ |$$$$$$$$/ $$ |       $$$$$$  | //
// $$ |  $$ |$$       |$$ |$$    $$/ $$       |$$ |      /     $$/  //
// $$/   $$/  $$$$$$$/ $$/ $$$$$$$/   $$$$$$$/ $$/       $$$$$$$/   //
//                         $$ |                                     //
//                         $$ |                                     //
//                         $$/                                      //
//                                                                  //
//////////////////////////////////////////////////////////////////////

stack_node* create_stack_node(const void* data, u64 stride) {
    stack_node* temp = (stack_node*)zmemory_allocate(sizeof(stack_node), MEMORY_TAG_STACK);
    temp->next = 0;
    temp->data = zmemory_allocate(stride, MEMORY_TAG_STACK);

    zmemory_copy(temp->data, data, stride);

    return temp;
}

void destroy_stack_node(stack_node* node, u64 stride) {
    if (!node) {
        return;
    }

    destroy_stack_node(node->next, stride);

    zmemory_free(node->data, stride, MEMORY_TAG_STACK);
    zmemory_free(node, sizeof(stack_node), MEMORY_TAG_STACK);
    return;
}