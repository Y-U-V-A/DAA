#include "next_smaller.h"
#include "stack.h"
#include "logger.h"
#include "common.h"

void next_smaller() {

    const i32 n = 20;
    i32 array[n];
    for (i32 i = 0; i < n; ++i) {
        array[i] = generate_random(100);
    }

    stack* stk = stack_create(i32);

    i32 result[n];

    for (i32 i = 2 * n - 1; i >= 0; --i) {

        while (stack_size(stk) != 0 && *(i32*)stack_top(stk) >= array[i % n]) {
            stack_pop(stk);
            result[i % n] = -1;
        }

        if (i < n) {
            if (stack_size(stk) == 0) {
                result[i % n] = -1;
            } else {
                result[i % n] = *(i32*)stack_top(stk);
            }
        }
        stack_push(stk, &array[i % n]);
    }

    stack_destroy(stk);

    const u32 buffer_size = 1000;
    char buffer[buffer_size];
    u32 offset = log_buffer(buffer, buffer_size, "array = ");

    for (i32 i = 0; i < n; ++i) {
        u32 x = log_buffer(buffer + offset, buffer_size - offset, "%d,", array[i]);
        offset += x;
    }
    buffer[offset] = '\0';
    LOGI("%s", buffer);

    offset = log_buffer(buffer, buffer_size, "result = ");
    for (i32 i = 0; i < n; ++i) {
        u32 x = log_buffer(buffer + offset, buffer_size - offset, "%d,", result[i]);
        offset += x;
    }
    buffer[offset] = '\0';
    LOGI("%s", buffer);
}

void next_smaller_run() {

    seed_random();

    for (i32 i = 0; i < 20; ++i) {
        next_smaller();
    }
}
