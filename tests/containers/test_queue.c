#include "test_queue.h"
#include "expect.h"
#include "test_manager.h"
#include "queue.h"

#include "queue.h"
#include "expect.h"

// Test structures
typedef struct {
    i32 priority;
    char description[32];
} Task;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

// Test integer operations
u32 test_queue_int() {
    queue* que = queue_create(i32);

    // Test initial state
    expect_should_be(0, queue_size(que));
    expect_should_be(0, (u64)queue_front(que));
    expect_should_be(0, (u64)queue_back(que));

    // Test push operations
    i32 values[] = {10, 20, 30, 40, 50};

    for (u32 i = 0; i < 5; i++) {
        queue_push(que, &values[i]);
        expect_should_be(i + 1, queue_size(que));
        expect_should_be(10, *(i32*)queue_front(que)); // First element should always be 10
        expect_should_be(values[i], *(i32*)queue_back(que));
    }

    // Test pop operations
    for (u32 i = 0; i < 5; i++) {
        expect_should_be(values[i], *(i32*)queue_front(que));
        queue_pop(que);
        expect_should_be(4 - i, queue_size(que));
    }

    queue_destroy(que);
    return true;
}

// Test double operations
u32 test_queue_double() {
    queue* que = queue_create(double);

    double values[] = {1.1, 2.2, 3.3, 4.4};

    // Push and verify
    for (i32 i = 0; i < 4; i++) {
        queue_push(que, &values[i]);
        expect_float_should_be(values[0], *(double*)queue_front(que));
        expect_float_should_be(values[i], *(double*)queue_back(que));
    }

    // Pop and verify FIFO order
    for (i32 i = 0; i < 4; i++) {
        expect_float_should_be(values[i], *(double*)queue_front(que));
        queue_pop(que);
    }

    queue_destroy(que);
    return true;
}

// Test Task structure operations
u32 test_queue_task() {
    queue* que = queue_create(Task);

    Task tasks[] = {
        {1, "High priority"},
        {2, "Medium priority"},
        {3, "Low priority"}};

    // Push and verify tasks
    for (i32 i = 0; i < 3; i++) {
        queue_push(que, &tasks[i]);
        Task* front = (Task*)queue_front(que);
        Task* back = (Task*)queue_back(que);

        expect_should_be(tasks[0].priority, front->priority); // First task always at front
        expect_should_be(tasks[i].priority, back->priority);  // Latest task at back
    }

    // Verify FIFO order during pop
    for (i32 i = 0; i < 3; i++) {
        Task* front = (Task*)queue_front(que);
        expect_should_be(tasks[i].priority, front->priority);
        queue_pop(que);
    }

    queue_destroy(que);
    return true;
}

// Test Vector3 structure operations
u32 test_queue_vector() {
    queue* que = queue_create(Vector3);

    Vector3 vectors[] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}};

    // Push and verify vectors
    for (i32 i = 0; i < 3; i++) {
        queue_push(que, &vectors[i]);
        Vector3* front = (Vector3*)queue_front(que);
        Vector3* back = (Vector3*)queue_back(que);

        // Verify front (should always be first vector)
        expect_float_should_be(vectors[0].x, front->x);
        expect_float_should_be(vectors[0].y, front->y);
        expect_float_should_be(vectors[0].z, front->z);

        // Verify back (should be current vector)
        expect_float_should_be(vectors[i].x, back->x);
        expect_float_should_be(vectors[i].y, back->y);
        expect_float_should_be(vectors[i].z, back->z);
    }

    queue_destroy(que);
    return true;
}

// Test edge cases
u32 test_queue_edge_cases() {
    queue* que = queue_create(i32);

    // Test operations on empty queue
    expect_should_be(0, (u64)queue_front(que));
    expect_should_be(0, (u64)queue_back(que));
    queue_pop(que); // Should handle gracefully

    // Test single element
    i32 value = 42;
    queue_push(que, &value);
    expect_should_be(1, queue_size(que));
    expect_should_be(value, *(i32*)queue_front(que));
    expect_should_be(value, *(i32*)queue_back(que));

    // Test push after pop
    queue_pop(que);
    expect_should_be(0, queue_size(que));
    queue_push(que, &value);
    expect_should_be(1, queue_size(que));
    expect_should_be(value, *(i32*)queue_front(que));

    queue_destroy(que);
    return true;
}

void test_queue_register() {
    test_manager_register_test(test_queue_int, "test_queue_int");
    test_manager_register_test(test_queue_double, "test_queue_double");
    test_manager_register_test(test_queue_task, "test_queue_task");
    test_manager_register_test(test_queue_vector, "test_queue_vector");
    test_manager_register_test(test_queue_edge_cases, "test_queue_edge_cases");
}