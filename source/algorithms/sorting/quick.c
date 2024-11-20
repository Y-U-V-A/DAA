#include "quick.h"
#include "clock.h"
#include "logger.h"
#include "common.h"

void quick_sort(i32* arr, i32 s, i32 e) {

    if (e <= s) {
        return;
    }

    i32 x = arr[s];
    i32 i = s, j = e;

    while (i < j) {

        while (i < e && arr[i] <= x) {
            ++i;
        }

        while (j > s && arr[j] > x) {
            --j;
        }

        if (i < j) {
            arr[j] ^= arr[i];
            arr[i] ^= arr[j];
            arr[j] ^= arr[i];
        }
    }

    if (s != j) {
        arr[s] ^= arr[j];
        arr[j] ^= arr[s];
        arr[s] ^= arr[j];
    }

    quick_sort(arr, s, j - 1);
    quick_sort(arr, j + 1, e);
}

void q_run() {
}

void quick_run() {

    LOGD("running quick sort... ");

    i32 num;
    LOGD("enter no of generate_random test cases to run");

    scan("%d", &num);

    LOGD("n = %d", num);

    seed_random();

    const i32 n = 30;
    i32 array[n];

    const i32 buff_size = 1024;
    char buffer[buff_size];

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < num; ++i) {

        for (i32 i = 0; i < n; ++i) {
            array[i] = generate_random(200);
        }

        clock clk;
        clock_start(&clk);

        quick_sort(array, 0, n - 1);

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