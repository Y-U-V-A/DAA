#include "bubble.h"
#include "common.h"
#include "clock.h"

void bubble_sort() {

    const int n = 30;
    i32 array[n];

    for (i32 i = 0; i < n; ++i) {
        array[i] = random(200);
    }

    clock clk;
    clock_start(&clk);

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

    clock_end(&clk);

    const int buff_size = 1000;
    char buffer[buff_size];
    u32 offset = log_buffer(buffer, buff_size, "time_s = %lf , sorted array = ", clk.elapsed);

    for (i32 i = 0; i < n; ++i) {

        u32 len = log_buffer(buffer + offset, buff_size - offset, "%d,", array[i]);
        offset += len;
    }

    buffer[offset] = 0;

    LOGD("%s", buffer);
}

void bubble_run() {

    LOGD("running bubble sort ");

    int n;
    LOGD("enter no of random test cases to run");

    scan("%d", &n);

    LOGD("n = %d", n);

    seed_random();

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        bubble_sort();
    }

    clock_end(&clk);

    LOGD("total time_s taken %lf", clk.elapsed);
}