#include "test_set.h"
#include "expect.h"
#include "test_manager.h"
#include "set.h"
#include "zmemory.h"
#include "utils.h"

typedef struct {
    i32 id;
    f32 value;
    char name[32];
} TestStruct;

// Comparison functions for different types
bool set_int_compare(const void* a, const void* b) {
    return *(const i32*)a > *(const i32*)b;
}

bool set_float_compare(const void* a, const void* b) {
    return *(const f32*)a > *(const f32*)b;
}

bool set_struct_compare(const void* a, const void* b) {
    return ((const TestStruct*)a)->id > ((const TestStruct*)b)->id;
}

// Helper function to create a test struct
TestStruct set_create_test_struct(i32 id, f32 value, const char* name) {
    TestStruct s;
    s.id = id;
    s.value = value;
    string_copy(s.name, name, 31);
    s.name[31] = '\0';
    return s;
}

// Integer Set Tests
u32 test_set_int_basic_operations() {
    set* int_set = set_create(i32, set_int_compare);

    // Test insertion
    i32 values[] = {5, 3, 7, 1, 9};
    for (i32 i = 0; i < 5; i++) {
        set_insert(int_set, &values[i]);
    }

    expect_should_be(5, set_length(int_set));

    // Test duplicate insertion
    set_insert(int_set, &values[0]);          // Try inserting 5 again
    expect_should_be(5, set_length(int_set)); // Size should not change

    // Test contains
    expect_should_be(true, set_contains(int_set, &values[0]));
    i32 non_existent = 100;
    expect_should_be(false, set_contains(int_set, &non_existent));

    // Test iteration (should be in ascending order)
    set_node* node = set_begin(int_set);
    i32 expected_order[] = {1, 3, 5, 7, 9};
    for (i32 i = 0; i < 5; i++) {
        expect_should_be(expected_order[i], *(i32*)node->data);
        if (i < 4) {
            node = set_next(int_set, node);
        }
    }

    // Test removal
    set_remove(int_set, &values[2]); // Remove 7
    expect_should_be(4, set_length(int_set));
    expect_should_be(false, set_contains(int_set, &values[2]));

    set_destroy(int_set);
    return true;
}

// Float Set Tests
u32 test_set_float_basic_operations() {
    set* float_set = set_create(f32, set_float_compare);

    // Test insertion
    f32 values[] = {1.5f, 2.5f, 1.5f, 3.5f, 2.5f}; // Note: duplicates
    for (i32 i = 0; i < 5; i++) {
        set_insert(float_set, &values[i]);
    }

    // Should only have 3 unique values
    expect_should_be(3, set_length(float_set));

    // Test iteration (should be ordered)
    set_node* node = set_begin(float_set);
    f32 expected_order[] = {1.5f, 2.5f, 3.5f};
    for (i32 i = 0; i < 3; i++) {
        expect_float_should_be(expected_order[i], *(f32*)node->data, EPSILON);
        if (i < 2) {
            node = set_next(float_set, node);
        }
    }

    set_destroy(float_set);
    return true;
}

// Struct Set Tests
u32 test_set_struct_basic_operations() {
    set* struct_set = set_create(TestStruct, set_struct_compare);

    // Create test structs
    TestStruct s1 = set_create_test_struct(1, 10.5f, "First");
    TestStruct s2 = set_create_test_struct(2, 20.5f, "Second");
    TestStruct s3 = s1;

    // Test insertion with duplicate IDs
    set_insert(struct_set, &s1);
    set_insert(struct_set, &s2);
    set_insert(struct_set, &s3); // Should update s1's values

    expect_should_be(2, set_length(struct_set)); // Should only have 2 unique elements

    // Verify the updated values
    set_node* node = set_begin(struct_set);
    TestStruct* retrieved = (TestStruct*)node->data;
    expect_should_be(1, retrieved->id);
    expect_float_should_be(10.5f, retrieved->value, EPSILON); // Should have s3's value

    set_destroy(struct_set);
    return true;
}

// Edge Cases Tests
u32 test_set_edge_cases() {
    set* int_set = set_create(i32, set_int_compare);

    // Test empty set operations
    expect_should_be(0, set_length(int_set));
    expect_should_be(false, set_contains(int_set, &(i32){1}));

    // Test single element
    i32 value = 42;
    set_insert(int_set, &value);
    expect_should_be(1, set_length(int_set));

    // Test removal of non-existent element
    i32 non_existent = 100;
    set_remove(int_set, &non_existent);
    expect_should_be(1, set_length(int_set)); // Size shouldn't change

    // Test removal of the only element
    set_remove(int_set, &value);
    expect_should_be(0, set_length(int_set));

    set_destroy(int_set);
    return true;
}

// Large Set Test
u32 test_set_large_operations() {
    set* int_set = set_create(i32, set_int_compare);

    // Insert 1000 numbers
    for (i32 i = 0; i < 1000; i++) {
        set_insert(int_set, &i);
    }

    expect_should_be(1000, set_length(int_set));

    // Verify ordering
    set_node* node = set_begin(int_set);
    i32 current = 0;
    while (current < 1000) {
        expect_should_be(current, *(i32*)node->data);
        current++;
        node = set_next(int_set, node);
    }

    // Remove every other number
    for (i32 i = 0; i < 1000; i += 2) {
        set_remove(int_set, &i);
    }

    expect_should_be(500, set_length(int_set));

    set_destroy(int_set);
    return true;
}

void register_set_tests() {

    test_manager_register_test(test_set_int_basic_operations, "test_set_int_basic_operations");
    test_manager_register_test(test_set_float_basic_operations, "test_set_float_basic_operations");
    test_manager_register_test(test_set_struct_basic_operations, "test_set_struct_basic_operations");
    test_manager_register_test(test_set_edge_cases, "test_set_edge_cases");
    test_manager_register_test(test_set_large_operations, "test_set_large_operations");
}