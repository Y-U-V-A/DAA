#include "priority_queue.h"
#include "darray.h"
#include "common.h"
#include "logger.h"

// implemented using darray
struct priority_queue {
    void* array;
    PFN_priority_cmp cmp;
};

priority_queue* _priority_queue_create(u64 stride, PFN_priority_cmp cmp) {

    priority_queue* temp = (priority_queue*)memory_allocate(sizeof(priority_queue), MEMORY_TAG_PRIORITY_QUEUE);
    temp->array = _darray_create(DARRAY_FIELD_LENGTH, stride);
    temp->cmp = cmp;

    return temp;
}

void priority_queue_destroy(priority_queue* prio_que) {
    if (prio_que) {
        if (prio_que->array) {
            darray_destroy(prio_que->array);
        }
        memory_free(prio_que, sizeof(priority_queue), MEMORY_TAG_PRIORITY_QUEUE);
    }
}

void priority_queue_push(priority_queue* prio_que, const void* data) {
    u64 stride = darray_stride(prio_que->array);

    prio_que->array = _darray_push_back(prio_que->array, data);
    u64 length = darray_size(prio_que->array);

    u64 c = length - 1;
    u64 p = (c - 1) / 2;

    void* temp = memory_allocate(stride, MEMORY_TAG_PRIORITY_QUEUE);

    while (c > 0 && prio_que->cmp(prio_que->array + p * stride, prio_que->array + c * stride) != true) {
        memory_copy(temp, prio_que->array + p * stride, stride);
        memory_move(prio_que->array + p * stride, prio_que->array + c * stride, stride);
        memory_copy(prio_que->array + c * stride, temp, stride);

        c = p;
        p = (c - 1) / 2;
    }

    memory_free(temp, stride, MEMORY_TAG_PRIORITY_QUEUE);
}

void priority_queue_pop(priority_queue* prio_que) {
    u64 stride = darray_stride(prio_que->array);
    u64 length = darray_size(prio_que->array);

    memory_move(prio_que->array, prio_que->array + (length - 1) * stride, stride);

    darray_pop_back(prio_que->array);

    length = darray_size(prio_que->array);

    u64 p = 0, c1 = 1, c2 = 2, maxi;

    void* temp = memory_allocate(stride, MEMORY_TAG_PRIORITY_QUEUE);

    while (c1 < length) {
        maxi = p;
        if (prio_que->cmp(prio_que->array + maxi * stride, prio_que->array + c1 * stride) != true)
            maxi = c1;
        if (c2 < length && prio_que->cmp(prio_que->array + maxi * stride, prio_que->array + c2 * stride) != true)
            maxi = c2;
        if (maxi == p)
            break;

        memory_copy(temp, prio_que->array + maxi * stride, stride);
        memory_move(prio_que->array + maxi * stride, prio_que->array + p * stride, stride);
        memory_copy(prio_que->array + p * stride, temp, stride);

        p = maxi;
        c1 = 2 * p + 1;
        c2 = 2 * p + 2;
    }
    memory_free(temp, stride, MEMORY_TAG_PRIORITY_QUEUE);
}

void* priority_queue_top(priority_queue* prio_que) {
    u32 length = darray_size(prio_que->array);
    if (length == 0) {
        LOGE("priority_queue_top : priority_queue has zero nodes");
        return 0;
    }

    return prio_que->array;
}

u64 priority_queue_size(priority_queue* prio_que) {
    return darray_size(prio_que->array);
}