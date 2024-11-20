#include "selection.h"
#include "clock.h"
#include "logger.h"
#include "common.h"

void selection_sort(i32* array, i32 n) {

    for (i32 i = 0; i < n; ++i) {
        for (i32 j = i + 1; j < n; ++j) {

            if (array[i] > array[j]) {

                array[i] ^= array[j];
                array[j] ^= array[i];
                array[i] ^= array[j];
            }
        }
    }
}

void selection_run() {

    LOGD("running selection sort... ");

    i32 num;
    LOGD("enter no of generate_random test cases to run");

    scan("%d", &num);

    LOGD("n = %d", num);

    seed_random();

    const i32 buff_size = 1024;
    char buffer[buff_size];

    const i32 n = 30;
    i32 array[n];

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < num; ++i) {

        for (i32 i = 0; i < n; ++i) {
            array[i] = generate_random(500);
        }

        clock clk;
        clock_start(&clk);

        selection_sort(array, n);

        clock_update(&clk);

        u32 offset = log_buffer(buffer, buff_size, "time = %lfs , sorted array = ", clk.elapsed);

        for (i32 i = 0; i < n; ++i) {

            offset += log_buffer(buffer + offset, buff_size - offset, "%d,", array[i]);
        }

        buffer[offset++] = '\n';
        buffer[offset++] = '\0';

        LOGD("%s", buffer);
    }

    clock_update(&clk);

    LOGD("total time_s taken %lf\n", clk.elapsed);
}