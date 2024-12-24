#include "quick.h"
#include "clock.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

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

    random_seed();

    LOGD("running quick sort ");

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
        quick_sort(array, 0, array_size - 1);
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