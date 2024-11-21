#include "bubble.h"
#include "clock.h"
#include "logger.h"
#include "common.h"

void bubble_sort(i32* array, i32 n) {

    for (i32 i = 0; i < n; ++i) {
        for (i32 j = 0; j < (n - i - 1); ++j) {

            if (array[j] > array[j + 1]) {

                // swap
                array[j] ^= array[j + 1];
                array[j + 1] ^= array[j];
                array[j] ^= array[j + 1];
            }
        }
    }
}

void bubble_run(const u32 buffer_size) {

    LOGD("running bubble sort ");

    i32 num;
    LOGD("enter no of generate_random test cases to run");

    scan("%d", &num);

    LOGD("n = %d", num);

    seed_random();

    char* buffer = memory_allocate(buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);

    const i32 n = 30;
    i32 array[n];

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < num; ++i) {

        for (i32 i = 0; i < n; ++i) {
            array[i] = generate_random(200);
        }

        clock clk;
        clock_start(&clk);

        bubble_sort(array, n);

        clock_update(&clk);

        u32 offset = log_buffer(buffer, buffer_size, "time = %lfs , sorted array = ", clk.elapsed);

        for (i32 i = 0; i < n; ++i) {

            offset += log_buffer(buffer + offset, buffer_size - offset, "%d,", array[i]);
        }
        buffer[offset++] = '\n';
        buffer[offset++] = '\0';

        LOGD("%s", buffer);
    }

    clock_update(&clk);

    memory_free(buffer, buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);

    LOGD("total time_s taken %lf\n", clk.elapsed);
}