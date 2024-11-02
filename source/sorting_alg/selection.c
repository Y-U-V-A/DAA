#include "selection.h"
#include "common.h"
#include "clock.h"

void selection_sort() {

    const int n = 30;
    i32 array[n];

    for (i32 i = 0; i < n; ++i) {
        array[i] = random(500);
    }

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        for (i32 j = i + 1; j < n; ++j) {

            if (array[i] > array[j]) {

                array[i] ^= array[j];
                array[j] ^= array[i];
                array[i] ^= array[j];
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

void selection_run() {

    LOGD("running selection sort... ");

    int n;
    LOGD("enter no of random test cases to run");

    scan("%d", &n);

    LOGD("n = %d", n);

    seed_random();

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        selection_sort();
    }

    clock_end(&clk);

    LOGD("total time_s taken %lf", clk.elapsed);
}