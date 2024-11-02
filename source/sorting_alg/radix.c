#include "quick.h"
#include "common.h"
#include "clock.h"
#include "darray.h"

void radix_sort(i32* arr, i32 n) {

    if (n == 0)
        return;

    i32 i;
    i32 maxi = arr[0];

    for (i = 0; i < n; ++i) {
        if (arr[i] > maxi) {
            maxi = arr[i];
        }
    }

    i32 place = 1, idx;
    i32* output = darray_reserve(n, i32);
    i32 count[10] = {0};

    do {

        for (i = 0; i < 10; ++i) {
            count[i] = 0;
        }

        for (i = 0; i < n; ++i) {
            count[(arr[i] / place) % 10] += 1;
        }

        for (i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        for (i = n - 1; i >= 0; --i) {
            idx = (arr[i] / place) % 10;
            output[count[idx] - 1] = arr[i];
            count[idx] -= 1;
        }

        for (i = 0; i < n; ++i) {
            arr[i] = output[i];
        }

        place *= 10;

    } while ((maxi / place) > 0);

    darray_destroy(output);
}

void r_run() {

    const int n = 30;
    i32 array[n];

    for (i32 i = 0; i < n; ++i) {
        array[i] = random(200);
    }

    clock clk;
    clock_start(&clk);

    radix_sort(array, n);

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

void radix_run() {

    LOGD("running radix sort... ");

    int n;
    LOGD("enter no of random test cases to run");

    scan("%d", &n);

    LOGD("n = %d", n);

    seed_random();

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        r_run();
    }

    clock_end(&clk);

    LOGD("total time_s taken %lf", clk.elapsed);
}