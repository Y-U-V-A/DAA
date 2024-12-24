#include "testing_freelist_allocator.h"
#include "zthread.h"
#include "expect.h"
#include "test_manager.h"
#include "zmemory.h"
#include "utils.h"
#include "clock.h"
#include "logger.h"
#include "freelist_allocator.h"

// Test helper functions
u32 freelist_verify_allocation(void* ptr, u64 size) {
    if (!ptr)
        return false;
    // Write pattern to verify memory
    zmemory_set(ptr, 0xAA, size);
    // Read back and verify
    unsigned char* bytes = (unsigned char*)ptr;
    for (u64 i = 0; i < size; i++) {
        if (bytes[i] != 0xAA)
            return false;
    }
    return true;
}

// Basic unit tests
u32 test_freelist_allocator_create_destroy() {
    freelist_allocator* allocator = freelist_allocator_create(1024);
    expect_should_not_be(0, (u64)allocator);
    freelist_allocator_destroy(allocator);
    return true;
}

u32 test_freelist_allocator_basic_alloc_free() {
    freelist_allocator* allocator = freelist_allocator_create(1024);
    void* ptr = freelist_allocator_allocate(allocator, 128);

    expect_should_not_be(0, (u64)ptr);
    expect_should_be(128 + freelist_allocator_header_size(),
                     freelist_allocator_used_memory(allocator));

    freelist_allocator_free(allocator, ptr);
    expect_should_be(0, freelist_allocator_used_memory(allocator));

    freelist_allocator_destroy(allocator);
    return true;
}

// Memory alignment tests
u32 test_freelist_allocator_alignment() {
    freelist_allocator* allocator = freelist_allocator_create(1024);

    // Test multiple allocations for 8-byte alignment
    void* ptr1 = freelist_allocator_allocate(allocator, 5);  // Odd size
    void* ptr2 = freelist_allocator_allocate(allocator, 10); // Even size

    expect_should_be(0, (u64)ptr1 % 8); // Should be 8-byte aligned
    expect_should_be(0, (u64)ptr2 % 8);

    freelist_allocator_free(allocator, ptr1);
    freelist_allocator_free(allocator, ptr2);
    freelist_allocator_destroy(allocator);
    return true;
}

// Edge cases
u32 test_freelist_allocator_edge_cases() {
    freelist_allocator* allocator = freelist_allocator_create(1024);

    // Test zero size allocation
    void* ptr1 = freelist_allocator_allocate(allocator, 0);
    expect_should_be(0, (u64)ptr1);

    // Test allocation larger than pool
    void* ptr2 = freelist_allocator_allocate(allocator, 2048);
    expect_should_be(0, (u64)ptr2);

    // Test null pointer free
    freelist_allocator_free(allocator, 0);

    // Test invalid address free (outside pool)
    char dummy;
    freelist_allocator_free(allocator, &dummy);

    freelist_allocator_destroy(allocator);
    return true;
}

// Fragmentation and coalescing tests
u32 test_freelist_allocator_fragmentation_coalescing() {
    freelist_allocator* allocator = freelist_allocator_create(1024);

    // Create fragmentation pattern
    void* ptr1 = freelist_allocator_allocate(allocator, 128);
    void* ptr2 = freelist_allocator_allocate(allocator, 128);
    void* ptr3 = freelist_allocator_allocate(allocator, 128);

    // Free middle block
    freelist_allocator_free(allocator, ptr2);

    // Try to allocate larger block than the hole
    void* ptr4 = freelist_allocator_allocate(allocator, 256);
    expect_should_not_be(0, (u64)ptr4);

    // Free adjacent blocks and test coalescing
    freelist_allocator_free(allocator, ptr1);
    freelist_allocator_free(allocator, ptr3);

    // Should now be able to allocate a larger block
    void* ptr5 = freelist_allocator_allocate(allocator, 256);
    expect_should_not_be(0, (u64)ptr5);

    freelist_allocator_free(allocator, ptr5);
    freelist_allocator_destroy(allocator);
    return true;
}

// Multithreading test structures
#define NUM_THREADS 4
#define ALLOCS_PER_THREAD 100
#define MAX_ALLOC_SIZE 128

typedef struct {
    freelist_allocator* allocator;
    u32 success;
} thread_data;

#ifdef ZWINDOWS
u32 freelist_thread_alloc_free(void* arg) {
#else
void* freelist_thread_alloc_free(void* arg) {
#endif
    thread_data* data = (thread_data*)arg;
    void* ptrs[ALLOCS_PER_THREAD];
    u64 sizes[ALLOCS_PER_THREAD];

    // Perform allocations
    for (i32 i = 0; i < ALLOCS_PER_THREAD; i++) {
        sizes[i] = (random_int(0, 1000) % MAX_ALLOC_SIZE) + 1;
        ptrs[i] = freelist_allocator_allocate(data->allocator, sizes[i]);
        if (!freelist_verify_allocation(ptrs[i], sizes[i])) {
            data->success = false;
            return 0;
        }
    }

    // Free in random_int(0,1000) order
    for (i32 i = ALLOCS_PER_THREAD - 1; i >= 0; i--) {
        if (ptrs[i]) {
            freelist_allocator_free(data->allocator, ptrs[i]);
        }
    }

    data->success = true;
    return 0;
}

u32 test_freelist_allocator_multithreaded() {
    freelist_allocator* allocator = freelist_allocator_create(1024 * 1024); // 1MB
    thread_data thread_args[NUM_THREADS];
    zthread threads[NUM_THREADS];

    for (i32 i = 0; i < NUM_THREADS; i++) {
        thread_args[i].allocator = allocator;
        thread_args[i].success = false;
        if (!zthread_create(freelist_thread_alloc_free, &thread_args[i], &threads[i])) {
            return false;
        }
    }

    if (!zthread_wait_on_all(threads, NUM_THREADS)) {
        return false;
    }

    for (i32 i = 0; i < NUM_THREADS; i++) {
        if (!thread_args[i].success) {
            return false;
        }
        zthread_destroy(&threads[i]);
    }

    // Verify final state
    // expect_should_be(0, freelist_allocator_used_memory(allocator));

    freelist_allocator_destroy(allocator);
    return true;
}

// Benchmark tests
u32 test_freelist_allocator_benchmark() {
    clock bench_clock;
    freelist_allocator* allocator = freelist_allocator_create(1024 * 1024); // 1MB
    void* ptrs[1000];

    // Benchmark allocation
    clock_set(&bench_clock);
    for (i32 i = 0; i < 1000; i++) {
        ptrs[i] = freelist_allocator_allocate(allocator, 512);
        if (!ptrs[i])
            return false;
    }
    clock_update(&bench_clock);
    LOGT("Allocation time for 1000 blocks: %f seconds", bench_clock.elapsed);

    // Benchmark fragmentation handling
    clock_set(&bench_clock);
    for (i32 i = 0; i < 1000; i += 2) {
        freelist_allocator_free(allocator, ptrs[i]);
    }
    for (i32 i = 1; i < 1000; i += 2) {
        freelist_allocator_free(allocator, ptrs[i]);
    }
    clock_update(&bench_clock);
    LOGT("Deallocation time with fragmentation: %f seconds", bench_clock.elapsed);

    freelist_allocator_destroy(allocator);
    return true;
}

// Reset functionality test
u32 test_freelist_allocator_reset() {
    freelist_allocator* allocator = freelist_allocator_create(1024);

    void* ptr1 = freelist_allocator_allocate(allocator, 256);
    void* ptr2 = freelist_allocator_allocate(allocator, 256);

    expect_should_not_be(0, (u64)ptr1);
    expect_should_not_be(0, (u64)ptr2);

    freelist_allocator_reset(allocator);
    expect_should_be(0, freelist_allocator_used_memory(allocator));

    // Should be able to allocate full size after reset
    void* ptr3 = freelist_allocator_allocate(allocator, 512);
    expect_should_not_be(0, (u64)ptr3);

    freelist_allocator_destroy(allocator);
    return true;
}

void register_freelist_tests() {

    test_manager_register_test(test_freelist_allocator_create_destroy, "test_freelist_allocator_create_destroy");
    test_manager_register_test(test_freelist_allocator_basic_alloc_free, "test_freelist_allocator_basic_alloc_free");
    test_manager_register_test(test_freelist_allocator_fragmentation_coalescing, "test_freelist_allocator_fragmentation_coalescing");
    test_manager_register_test(test_freelist_allocator_multithreaded, "test_freelist_allocator_multithreaded");
    test_manager_register_test(test_freelist_allocator_alignment, "test_freelist_allocator_alignment");
    test_manager_register_test(test_freelist_allocator_edge_cases, "test_freelist_allocator_edge_cases");
    test_manager_register_test(test_freelist_allocator_reset, "test_freelist_allocator_reset");
    test_manager_register_test(test_freelist_allocator_benchmark, "test_freelist_allocator_benchmark");
}
