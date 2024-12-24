#include "insertion.h"
#include "clock.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

void insertion_sort(i32* array, i32 n) {

    for (i32 i = 1; i < n; ++i) {
        i32 x = array[i], j = i;

        while (j && array[j - 1] > x) {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = x;
    }
}

void insertion_run() {

    random_seed();

    LOGD("running insertion sort ");

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
        insertion_sort(array, array_size);
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