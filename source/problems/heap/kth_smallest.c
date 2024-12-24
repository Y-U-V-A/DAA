#include "kth_smallest.h"
#include "priority_queue.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

bool int_cmp_smallest(const void* parent, const void* child) {
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

    random_seed();

    const i32 n = random_int(0, 30);
    i32 arr[n];

    char* buffer = zmemory_allocate(n * sizeof(char) + 1024, MEMORY_TAG_ALGORITHM);

    u32 offset = log_buffer(buffer, n, "size = %d ,array = ", n);

    for (i32 i = 0; i < n; ++i) {
        arr[i] = random_int(0, 200);
        offset += log_buffer(buffer + offset, n - offset, "%d,", arr[i]);
    }
    buffer[offset++] = '\n';
    buffer[offset++] = '\0';
    LOGI("%s", buffer);

    i32 times = random_int(0, n);
    for (i32 i = 0; i < times; ++i) {
        i32 k = random_int(0, n - 1);
        i32 result = kth_smallest(arr, n, k);
        LOGI("%dth smallest = %d", k, result);
    }

    zmemory_free(buffer, n * sizeof(char), MEMORY_TAG_ALGORITHM);
}