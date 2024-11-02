#include "merge.h"
#include "common.h"
#include "clock.h"
#include "darray.h"

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

void m_run() {

    const int n = 30;
    i32 array[n];

    for (i32 i = 0; i < n; ++i) {
        array[i] = random(200);
    }

    clock clk;
    clock_start(&clk);

    merge_sort(array, 0, n - 1);

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

void merge_run() {

    LOGD("running merge sort... ");

    int n;
    LOGD("enter no of random test cases to run");

    scan("%d", &n);

    LOGD("n = %d", n);

    seed_random();

    clock clk;
    clock_start(&clk);

    for (i32 i = 0; i < n; ++i) {
        m_run();
    }

    clock_end(&clk);

    LOGD("total time_s taken %lf", clk.elapsed);
}