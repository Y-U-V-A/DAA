#include "test_queue.h"
#include "expect.h"
#include "test_manager.h"
#include "priority_queue.h"

b8 prio_que_int_compare(const void* parent, const void* child) {
    return *(const i32*)parent >= *(const i32*)child;
}

// Test struct for complex data type testing
typedef struct TestItem {
    i32 priority;
    const char* data;
} TestItem;

// Compare function for TestItem (max heap based on priority)
b8 prio_que_item_compare(const void* parent, const void* child) {
    const TestItem* p = (const TestItem*)parent;
    const TestItem* c = (const TestItem*)child;
    return p->priority >= c->priority;
}

u32 test_priority_queue_basic_operations() {
    // Create a priority queue for integers
    priority_queue* pq = _priority_queue_create(sizeof(i32), prio_que_int_compare);

    // Test initial size
    expect_should_be(0, priority_queue_size(pq));

    // Push some values
    i32 values[] = {5, 3, 8, 1, 2};
    for (i32 i = 0; i < 5; i++) {
        priority_queue_push(pq, &values[i]);
    }

    // Test size after pushing
    expect_should_be(5, priority_queue_size(pq));

    // Test top value (should be 8 as it's a max heap)
    i32* top = (i32*)priority_queue_top(pq);
    expect_should_be(8, *top);

    // Test popping values (should come out in descending order)
    i32 expected[] = {8, 5, 3, 2, 1};
    for (i32 i = 0; i < 5; i++) {
        top = (i32*)priority_queue_top(pq);
        expect_should_be(expected[i], *top);
        priority_queue_pop(pq);
    }

    // Test final size
    expect_should_be(0, priority_queue_size(pq));

    priority_queue_destroy(pq);
    return true;
}

u32 test_priority_queue_struct_operations() {
    // Create a priority queue for TestItem structs
    priority_queue* pq = _priority_queue_create(sizeof(TestItem), prio_que_item_compare);

    // Test initial size
    expect_should_be(0, priority_queue_size(pq));

    // Create test items
    TestItem items[] = {
        {3, "Medium"},
        {5, "High"},
        {1, "Low"},
        {4, "Important"},
        {2, "Normal"}};

    // Push items
    for (i32 i = 0; i < 5; i++) {
        priority_queue_push(pq, &items[i]);
    }

    // Test size after pushing
    expect_should_be(5, priority_queue_size(pq));

    // Test top item (should be priority 5)
    TestItem* top = (TestItem*)priority_queue_top(pq);
    expect_should_be(5, top->priority);

    // Expected priorities in order
    i32 expected_priorities[] = {5, 4, 3, 2, 1};

    // Pop and verify order
    for (i32 i = 0; i < 5; i++) {
        top = (TestItem*)priority_queue_top(pq);
        expect_should_be(expected_priorities[i], top->priority);
        priority_queue_pop(pq);
    }

    // Test final size
    expect_should_be(0, priority_queue_size(pq));

    priority_queue_destroy(pq);
    return true;
}

void register_priority_queue_tests() {
    test_manager_register_test(test_priority_queue_basic_operations, "test_priority_queue_basic_operations");
    test_manager_register_test(test_priority_queue_struct_operations, "test_priority_queue_struct_operations");
}
