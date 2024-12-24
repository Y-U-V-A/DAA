#include "testing_linear_allocator.h"
#include "zthread.h"
#include "expect.h"
#include "test_manager.h"
#include "zmemory.h"
#include "utils.h"
#include "clock.h"
#include "linear_allocator.h"

// Test structure to pass data to threads
typedef struct thread_test_data {
    linear_allocator* allocator;
    u64 iterations;
    u64 alloc_size;
} thread_test_data;

// Unit Tests
u32 linear_test_create_destroy() {
    linear_allocator* allocator = linear_allocator_create(1024);
    if (!allocator)
        return false;

    expect_should_be(0, linear_allocator_used_memory(allocator));
    expect_should_be(1024, linear_allocator_unused_memory(allocator));

    linear_allocator_destroy(allocator);
    return true;
}

u32 linear_test_basic_allocation() {
    linear_allocator* allocator = linear_allocator_create(1024);

    void* block1 = linear_allocator_allocate(allocator, 256);
    expect_should_not_be(0, block1);
    expect_should_be(256, linear_allocator_used_memory(allocator));

    void* block2 = linear_allocator_allocate(allocator, 256);
    expect_should_not_be(0, block2);
    expect_should_be(512, linear_allocator_used_memory(allocator));

    linear_allocator_reset(allocator);
    expect_should_be(0, linear_allocator_used_memory(allocator));

    linear_allocator_destroy(allocator);
    return true;
}

// Alignment Tests
u32 linear_test_alignments() {
    linear_allocator* allocator = linear_allocator_create(1024);

    // Test 8-byte alignment
    void* block8 = linear_allocator_allocate_aligned_8(allocator, 10);
    expect_should_be(0, ((u64)block8 & 7));

    // Test 16-byte alignment
    void* block16 = linear_allocator_allocate_aligned_16(allocator, 10);
    expect_should_be(0, ((u64)block16 & 15));

    // Test 32-byte alignment
    void* block32 = linear_allocator_allocate_aligned_32(allocator, 10);
    expect_should_be(0, ((u64)block32 & 31));

    // Test 64-byte alignment
    void* block64 = linear_allocator_allocate_aligned_64(allocator, 10);
    expect_should_be(0, ((u64)block64 & 63));

    linear_allocator_destroy(allocator);
    return true;
}

// Edge Cases
u32 linear_test_edge_cases() {
    linear_allocator* allocator = linear_allocator_create(1024);

    // Test null/invalid params
    expect_should_be(0, linear_allocator_allocate(0, 100));
    expect_should_be(0, linear_allocator_allocate(allocator, 0));

    // Test allocation larger than available space
    expect_should_be(0, linear_allocator_allocate(allocator, 2048));

    // Test allocation that exactly fits
    void* block = linear_allocator_allocate(allocator, 1024);
    expect_should_not_be(0, block);
    expect_should_be(1024, linear_allocator_used_memory(allocator));

    // Test allocation after full
    expect_should_be(0, linear_allocator_allocate(allocator, 1));

    linear_allocator_destroy(allocator);
    return true;
}

// Multi-threaded test function
#ifdef ZWINDOWS
u32 linear_thread_alloc_free(void* data) {
#else
void* linear_thread_alloc_free(void* data) {
#endif
    thread_test_data* test_data = (thread_test_data*)data;

    for (u64 i = 0; i < test_data->iterations; i++) {
        void* block = linear_allocator_allocate(test_data->allocator, test_data->alloc_size);
        if (!block) {
            LOGE("Thread allocation failed at iteration %llu", i);
            return 0;
        }
    }

    return 0;
}

u32 linear_test_multi_threaded() {
    const u64 THREAD_COUNT = 4;
    const u64 ITERATIONS = 1000;
    const u64 ALLOC_SIZE = 64;

    // Create allocator with enough space for all threads
    linear_allocator* allocator = linear_allocator_create(THREAD_COUNT * ITERATIONS * ALLOC_SIZE * 2); // *2 for alignment padding

    thread_test_data test_data = {
        .allocator = allocator,
        .iterations = ITERATIONS,
        .alloc_size = ALLOC_SIZE};

    zthread threads[THREAD_COUNT];

    // Create threads
    for (u64 i = 0; i < THREAD_COUNT; i++) {
        if (!zthread_create(linear_thread_alloc_free, &test_data, &threads[i])) {
            LOGE("Failed to create thread %llu", i);
            return false;
        }
    }

    // Wait for all threads to complete
    if (!zthread_wait_on_all(threads, THREAD_COUNT)) {
        LOGE("Failed to wait for threads");
        return false;
    }

    // Verify memory usage
    expect_should_not_be(0, linear_allocator_used_memory(allocator));

    linear_allocator_destroy(allocator);
    return true;
}

// Stress test with mixed alignments
u32 linear_test_mixed_alignments() {
    linear_allocator* allocator = linear_allocator_create(1024 * 1024);

    for (i32 i = 0; i < 50; i++) {
        void* block8 = linear_allocator_allocate_aligned_8(allocator, 7);
        void* block16 = linear_allocator_allocate_aligned_16(allocator, 9);
        void* block32 = linear_allocator_allocate_aligned_32(allocator, 17);
        void* block64 = linear_allocator_allocate_aligned_64(allocator, 33);

        expect_should_be(0, ((u64)block8 & 7));
        expect_should_be(0, ((u64)block16 & 15));
        expect_should_be(0, ((u64)block32 & 31));
        expect_should_be(0, ((u64)block64 & 63));
    }

    linear_allocator_destroy(allocator);
    return true;
}

// Benchmark test
u32 linear_test_benchmark() {
    const u64 TEST_SIZE = 1024 * 1024 * 64; // 64MB
    const u64 ITERATIONS = 10000;
    const u64 BLOCK_SIZE = 1024; // 1KB

    linear_allocator* allocator = linear_allocator_create(TEST_SIZE);
    clock benchmark_clock;

    // Benchmark sequential allocations
    clock_set(&benchmark_clock);

    for (u64 i = 0; i < ITERATIONS; i++) {
        void* block = linear_allocator_allocate(allocator, BLOCK_SIZE);
        if (!block) {
            if (i < ITERATIONS / 2) {
                LOGE("Benchmark allocation failed too early at iteration %llu", i);
                return false;
            }
            break;
        }
    }

    clock_update(&benchmark_clock);
    f64 allocation_time = benchmark_clock.elapsed;

    // Benchmark reset
    clock_set(&benchmark_clock);
    linear_allocator_reset(allocator);
    clock_update(&benchmark_clock);
    f64 reset_time = benchmark_clock.elapsed;

    LOGI("Benchmark Results:");
    LOGI("Sequential Allocation Time: %f seconds", allocation_time);
    LOGI("Reset Time: %f seconds", reset_time);
    LOGI("Allocations per second: %f", ITERATIONS / allocation_time);

    linear_allocator_destroy(allocator);
    return true;
}

void register_linear_tests() {

    test_manager_register_test(linear_test_create_destroy, "linear_test_create_destroy");
    test_manager_register_test(linear_test_basic_allocation, "linear_test_basic_allocation");
    test_manager_register_test(linear_test_alignments, "linear_test_alignments");
    test_manager_register_test(linear_test_edge_cases, "linear_test_edge_cases");
    test_manager_register_test(linear_test_multi_threaded, "linear_test_multi_threaded");
    test_manager_register_test(linear_test_mixed_alignments, "linear_test_mixed_alignments");
    test_manager_register_test(linear_test_benchmark, "linear_test_benchmark");
}