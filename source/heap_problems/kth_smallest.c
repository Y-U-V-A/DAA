#include "kth_smallest.h"
#include "priority_queue.h"
#include "common.h"

b8 int_cmp_smallest(const void* parent, const void* child) {
    return *(i32*)parent >= *(i32*)child;
}

i32 kth_smallest(i32* arr, i32 n, i32 k) {

    if (k == 0)
        return -1;

    priority_queue* prio_que = priority_queue_create(i32, int_cmp_smallest);
    i32 i;
    for (i = 0; i < k; ++i) {
        priority_queue_push(prio_que, &arr[i]);
    }

    for (; i < n; ++i) {

        if (arr[i] < *(i32*)priority_queue_top(prio_que)) {
            priority_queue_pop(prio_que);
            priority_queue_push(prio_que, &arr[i]);
        }
    }

    i32 result = *(i32*)priority_queue_top(prio_que);

    priority_queue_destroy(prio_que);

    return result;
}

void kth_smallest_run() {

    seed_random();

    const i32 n = random(30);
    i32 arr[n];

    const i32 buffer_size = 1000;
    char buffer[buffer_size];

    u32 offset = log_buffer(buffer, buffer_size, "size = %d ,array = ", n);

    for (i32 i = 0; i < n; ++i) {
        arr[i] = random(200);

        u32 x = log_buffer(buffer + offset, buffer_size - offset, "%d,", arr[i]);
        offset += x;
    }
    buffer[offset] = '\0';
    LOGI("%s", buffer);

    i32 times = random(n);
    for (i32 i = 0; i < times; ++i) {
        i32 k = random(n - 1);
        i32 result = kth_smallest(arr, n, k);
        LOGI("%dth smallest = %d", k, result);
    }
}