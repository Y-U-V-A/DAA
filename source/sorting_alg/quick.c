#include "quick.h"
#include "common.h"
#include "clock.h"

void quick_sort(i32* arr, i32 s, i32 e) {

    if (e <= s) {
        return;
    }

    i32 x = arr[s];
    i32 i = s, j = e;

    while (i < j) {

        while (i <= e && arr[i] <= x) {
            i++;
        }

        while (j > s && arr[j] > x) {
            j--;
        }

        if (i < j) {
            i32 temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    i32 temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;

    quick_sort(arr, s, j - 1);
    quick_sort(arr, j + 1, e);
}

void q_run() {

    const int n = 30;
    i32 array[n];

    for (i32 i = 0; i < n; ++i) {
        array[i] = random(200);
    }

    clock clk;
    clock_start(&clk);

    quick_sort(array, 0, n - 1);

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

void quick_run() {

    LOGD("running quick sort... ");

    int n;
    LOGD("enter no of random test cases to run");

    scan("%d", &n);

    LOGD("n = %d", n);

    seed_random();

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        q_run();
    }

    clock_end(&clk);

    LOGD("total time_s taken %lf", clk.elapsed);
}