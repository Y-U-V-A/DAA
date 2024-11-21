#include "merge.h"
#include "clock.h"
#include "darray.h"
#include "logger.h"
#include "common.h"

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

void merge_run(const u32 buffer_size) {

    LOGD("running merge sort... ");

    i32 num;
    LOGD("enter no of generate_random test cases to run");

    scan("%d", &num);

    LOGD("n = %d", num);

    seed_random();

    const i32 n = 30;
    i32 array[n];

    char* buffer = memory_allocate(buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < num; ++i) {

        for (i32 i = 0; i < n; ++i) {
            array[i] = generate_random(200);
        }

        clock clk;
        clock_start(&clk);

        merge_sort(array, 0, n - 1);

        clock_update(&clk);

        u32 offset = log_buffer(buffer, buffer_size, "time = %lfs , sorted array = ", clk.elapsed);

        for (i32 i = 0; i < n; ++i) {

            offset += log_buffer(buffer + offset, buffer_size - offset, "%d,", array[i]);
        }

        buffer[offset++] = '\n';
        buffer[offset++] = '\0';

        LOGD("%s", buffer);
    }

    clock_update(&clk);

    memory_free(buffer, buffer_size * sizeof(char), MEMORY_TAG_ALGORITHM);

    LOGD("total time_s taken %lf\n", clk.elapsed);
}