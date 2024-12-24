#include "merge.h"
#include "clock.h"
#include "darray.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

void merge_sort(i32* arr, i32 s, i32 e) {

    if (e <= s) {
        return;
    }

    i32 m = (s + e) / 2;

    merge_sort(arr, s, m);
    merge_sort(arr, m + 1, e);

    i32* temp = darray_create(i32);

    i32 i = s, j = m + 1;

    while (i <= m && j <= e) {
        if (arr[i] <= arr[j]) {
            darray_push_back(temp, arr[i]);
            i++;
        } else {
            darray_push_back(temp, arr[j]);
            j++;
        }
    }
    while (i <= m) {
        darray_push_back(temp, arr[i]);
        i++;
    }
    while (j <= e) {
        darray_push_back(temp, arr[j]);
        j++;
    }

    i32 k = 0;

    for (i32 g = s; g <= e; ++g) {
        arr[g] = temp[k++];
    }

    darray_destroy(temp);
}

void merge_run() {

    random_seed();

    LOGD("running merge sort ");

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
        merge_sort(array, 0, array_size - 1);
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