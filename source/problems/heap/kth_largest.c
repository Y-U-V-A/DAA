#include "kth_largest.h"
#include "priority_queue.h"
#include "logger.h"
#include "common.h"

bool int_cmp_largest(const void* parent, const void* child) {
    return *(i32*)parent <= *(i32*)child;
}

i32 kth_largest(i32* arr, i32 n, i32 k) {

    if (k == 0)
        return -1;

    priority_queue* prio_que = priority_queue_create(i32, int_cmp_largest);
    i32 i;
    for (i = 0; i < k; ++i) {
        priority_queue_push(prio_que, &arr[i]);
    }

    for (; i < n; ++i) {

        if (arr[i] > *(i32*)priority_queue_top(prio_que)) {
            priority_queue_pop(prio_que);
            priority_queue_push(prio_que, &arr[i]);
        }
    }

    i32 result = *(i32*)priority_queue_top(prio_que);

    priority_queue_destroy(prio_que);

    return result;
}

void kth_largest_run(const u32 buffer_size) {

    seed_random();

    const i32 n = generate_random(30);
    i32 arr[n];

    char* buffer = memory_allocate(buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);

    u32 offset = log_buffer(buffer, buffer_size, "size = %d ,array = ", n);

    for (i32 i = 0; i < n; ++i) {
        arr[i] = generate_random(200);
        offset += log_buffer(buffer + offset, buffer_size - offset, "%d,", arr[i]);
    }
    buffer[offset++] = '\n';
    buffer[offset++] = '\0';
    LOGI("%s", buffer);

    i32 times = generate_random(n);
    for (i32 i = 0; i < times; ++i) {
        i32 k = generate_random(n - 1);
        i32 result = kth_largest(arr, n, k);
        LOGI("%dth largest = %d", k, result);
    }

    memory_free(buffer, buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);
}