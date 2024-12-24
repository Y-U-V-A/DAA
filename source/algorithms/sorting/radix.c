#include "quick.h"
#include "clock.h"
#include "darray.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

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
}

void radix_run() {

    random_seed();

    LOGD("running radix sort ");

    i32 testcases;
    LOGD("enter no of random testcases to run ");
    input("%d", &testcases);
    i32 array_size;
    LOGD("enter array size ");
    input("%d", &array_size);

    i32* array = zmemory_allocate(array_size * sizeof(i32), MEMORY_TAG_ALGORITHM);

    f64 time_taken = 0.0;

    for (i32 i = 0; i < testcases; ++i) {

        for (i32 i = 0; i < array_size; ++i) {
            array[i] = random_int(0, 200);
        }

        clock clk;
        clock_set(&clk);
        radix_sort(array, array_size);
        clock_update(&clk);
        time_taken += clk.elapsed;

        LOGZ("time = %lfs , sorted array = ", clk.elapsed);
        for (i32 i = 0; i < array_size; ++i) {
            LOGZ("%d,", array[i]);
        }
        LOGZ("\n");
    }

    zmemory_free(array, array_size * sizeof(i32), MEMORY_TAG_ALGORITHM);

    LOGD("avarage time taken %lf\n", time_taken / testcases);
}