#include "bubble.h"
#include "clock.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

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

void bubble_run() {
    random_seed();

    LOGD("running bubble sort ");

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
        bubble_sort(array, array_size);
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
