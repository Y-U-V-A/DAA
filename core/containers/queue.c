#include "queue.h"
#include "zmemory.h"
#include "logger.h"

////////////////////////////////////////////////////////
//   ______   __    __   ______   __    __   ______   //
//  /      \ /  |  /  | /      \ /  |  /  | /      \  //
// /$$$$$$  |$$ |  $$ |/$$$$$$  |$$ |  $$ |/$$$$$$  | //
// $$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$ |$$    $$ | //
// $$ \__$$ |$$ \__$$ |$$$$$$$$/ $$ \__$$ |$$$$$$$$/  //
// $$    $$ |$$    $$/ $$       |$$    $$/ $$       | //
//  $$$$$$$ | $$$$$$/   $$$$$$$/  $$$$$$/   $$$$$$$/  //
//       $$ |                                         //
//       $$ |                                         //
//       $$/                                          //
//                                                    //
////////////////////////////////////////////////////////

typedef struct queue_node {
    void* data;
    struct queue_node* next;
} queue_node;

typedef struct queue {
    queue_node* front;
    queue_node* back;
    u64 size;
    u64 stride;
} queue;

queue_node* create_queue_node(const void* data, u64 stride);
void destroy_queue_node(queue_node* node, u64 stride);

queue* _queue_create(u64 stride) {
    queue* temp = (queue*)zmemory_allocate(sizeof(queue), MEMORY_TAG_QUEUE);
    temp->back = 0;
    temp->front = 0;
    temp->size = 0;
    temp->stride = stride;

    return temp;
}

void queue_destroy(queue* que) {
    if (que->front) {
        destroy_queue_node(que->front, que->stride);
    }
    zmemory_free(que, sizeof(queue), MEMORY_TAG_QUEUE);
}

void queue_push(queue* que, const void* data) {
    queue_node* node = create_queue_node(data, que->stride);

    if (que->front == 0) {
        que->front = node;
        que->back = node;
    } else {
        que->back->next = node;
        que->back = node;
    }
    que->size += 1;
}

void queue_pop(queue* que) {

    if (que->front == 0) {
        LOGW("queue_pop : queue has zero nodes");
        return;
    }

    queue_node* temp = que->front;
    que->front = temp->next;
    temp->next = 0;
    destroy_queue_node(temp, que->stride);

    que->size -= 1;
}

void* queue_front(queue* que) {
    if (que->front == 0) {
        LOGW("queue_front : queue has zero nodes");
        return 0;
    }

    return que->front->data;
}

void* queue_back(queue* que) {
    if (que->front == 0) {
        LOGW("queue_back : queue has zero nodes");
        return 0;
    }

    return que->back->data;
}

u64 queue_length(queue* que) {
    return que->size;
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

queue_node* create_queue_node(const void* data, u64 stride) {
    queue_node* temp = (queue_node*)zmemory_allocate(sizeof(queue_node), MEMORY_TAG_QUEUE);
    temp->next = 0;
    temp->data = zmemory_allocate(stride, MEMORY_TAG_QUEUE);

    zmemory_copy(temp->data, data, stride);

    return temp;
}

void destroy_queue_node(queue_node* node, u64 stride) {

    if (!node) {
        return;
    }

    destroy_queue_node(node->next, stride);

    zmemory_free(node->data, stride, MEMORY_TAG_QUEUE);
    zmemory_free(node, sizeof(queue_node), MEMORY_TAG_QUEUE);
    return;
}