#include "test_manager.h"
#include "darray.h"
#include "clock.h"
#include "common.h"

typedef struct test_entry {
    PFN_test func;
    const char* msg;
} test_entry;

// darray
static test_entry* test_manager;

void test_manager_init() {
    test_manager = darray_create(test_entry);
}

void test_manager_destroy() {
    if (test_manager) {
        darray_destroy(test_manager);
        test_manager = 0;
    }
}

void test_manager_register_test(PFN_test func, const char* msg) {
    test_entry temp;
    temp.func = func;
    temp.msg = msg;

    darray_push_back(test_manager, temp);
}

void test_manager_run() {

    LOGD("testing started...");

    clock clk_total;
    clock_start(&clk_total);

    u64 length = darray_size(test_manager);

    u32 passed = 0;
    u32 failed = 0;
    u32 skipped = 0;

    for (u64 i = 0; i < length; ++i) {

        clock clk;
        clock_start(&clk);

        u32 result = test_manager[i].func();

        clock_end(&clk);

        if (result == false) {

            failed += 1;
            LOGE("test failed : %s , time_s = %lf ", test_manager[i].msg, clk.elapsed);

        } else if (result == true) {

            passed += 1;
            LOGT("test passed : %s ,time_s = %lf ", test_manager[i].msg, clk.elapsed);

        } else {

            skipped += 1;
            LOGW("test skipped : %s ,time_s = %lf", test_manager[i].msg, clk.elapsed);
        }
        // memory_state_log();
    }

    clock_end(&clk_total);

    LOGD("testing ended...");
    LOGD("testing result / total = %u / passed = %u / failed = %u / skipped = %u / total_time_s = %lf",
         length, passed, failed, skipped, clk_total.elapsed);
}