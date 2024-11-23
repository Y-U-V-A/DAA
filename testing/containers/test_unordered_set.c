#include "test_set.h"
#include "expect.h"
#include "test_manager.h"
#include "unordered_set.h"

typedef struct test_struct {
    i32 id;
    f32 value;
    char name[32];
} test_struct;

u32 test_unset_int() {
    unordered_set* set = unordered_set_create(i32, 0);

    // Test insertion
    i32 value1 = 10;
    i32 value2 = 20;
    i32 value3 = 30;

    unordered_set_insert(set, &value1);
    unordered_set_insert(set, &value2);
    unordered_set_insert(set, &value3);

    // Test size
    expect_should_be(3, unordered_set_size(set));

    // Test duplicate insertion
    unordered_set_insert(set, &value1); // Should not increase size
    expect_should_be(3, unordered_set_size(set));

    // Test contains
    expect_should_be(true, unordered_set_contains(set, &value1));
    expect_should_be(true, unordered_set_contains(set, &value2));
    expect_should_be(true, unordered_set_contains(set, &value3));

    i32 nonexistent = 100;
    expect_should_be(false, unordered_set_contains(set, &nonexistent));

    // Test removal
    unordered_set_remove(set, &value2);
    expect_should_be(2, unordered_set_size(set));
    expect_should_be(false, unordered_set_contains(set, &value2));

    unordered_set_destroy(set);
    return true;
}

u32 test_unset_float() {
    unordered_set* set = unordered_set_create(f32, 0);

    // Test insertion
    f32 value1 = 3.14f;
    f32 value2 = 2.718f;
    f32 value3 = 1.414f;

    unordered_set_insert(set, &value1);
    unordered_set_insert(set, &value2);
    unordered_set_insert(set, &value3);

    // Test size
    expect_should_be(3, unordered_set_size(set));

    // Test contains with floating point values
    expect_should_be(true, unordered_set_contains(set, &value1));

    f32 nonexistent = 99.99f;
    expect_should_be(false, unordered_set_contains(set, &nonexistent));

    // Test duplicate insertion with floating point
    unordered_set_insert(set, &value2); // Should not increase size
    expect_should_be(3, unordered_set_size(set));

    unordered_set_destroy(set);
    return true;
}

u32 test_unset_struct() {
    unordered_set* set = unordered_set_create(test_struct, 0);

    // Test insertion with structs
    test_struct value1 = {1, 1.1f, "First"};
    test_struct value2 = {2, 2.2f, "Second"};
    test_struct value3 = {3, 3.3f, "Third"};

    unordered_set_insert(set, &value1);
    unordered_set_insert(set, &value2);
    unordered_set_insert(set, &value3);

    // Test size
    expect_should_be(3, unordered_set_size(set));

    // Test contains with struct
    expect_should_be(true, unordered_set_contains(set, &value1));

    // Test duplicate insertion with struct
    unordered_set_insert(set, &value1); // Should not increase size
    expect_should_be(3, unordered_set_size(set));

    // Test struct with same id but different values
    test_struct similar_value = {1, 9.9f, "Different"};
    unordered_set_insert(set, &similar_value); // Should be treated as different
    expect_should_be(4, unordered_set_size(set));

    unordered_set_destroy(set);
    return true;
}

u32 test_unset_resize_functionality() {
    unordered_set* set = unordered_set_create(i32, 0);

    // Insert enough elements to trigger resize
    for (i32 i = 0; i < 30; i++) {
        unordered_set_insert(set, &i);
    }

    // Verify all elements are still accessible after resize
    for (i32 i = 0; i < 30; i++) {
        expect_should_be(true, unordered_set_contains(set, &i));
    }

    // Verify capacity has increased
    expect_should_be(true, unordered_set_capacity(set) > UNORDERED_SET_DEFAULT_SIZE);

    // Verify size is correct
    expect_should_be(30, unordered_set_size(set));

    unordered_set_destroy(set);
    return true;
}

u32 test_unset_collision_handling() {
    unordered_set* set = unordered_set_create(i32, 0);

    // Create enough elements to ensure some collisions
    const i32 num_elements = 50;
    for (i32 i = 0; i < num_elements; i++) {
        unordered_set_insert(set, &i);
    }

    // Verify all elements are still accessible despite collisions
    for (i32 i = 0; i < num_elements; i++) {
        expect_should_be(true, unordered_set_contains(set, &i));
    }

    // Test removal of elements in collision chain
    for (i32 i = 0; i < num_elements; i += 2) {
        unordered_set_remove(set, &i);
        expect_should_be(false, unordered_set_contains(set, &i));
        // Verify adjacent elements still exist
        if (i + 1 < num_elements) {
            i32 x = i + 1;
            expect_should_be(true, unordered_set_contains(set, &x));
        }
    }

    unordered_set_destroy(set);
    return true;
}

void register_unordered_set_tests() {

    test_manager_register_test(test_unset_int, "test_unset_int");
    test_manager_register_test(test_unset_float, "test_unset_float");
    test_manager_register_test(test_unset_struct, "test_unset_struct");
    test_manager_register_test(test_unset_resize_functionality, "test_unset_resize_functionality");
    test_manager_register_test(test_unset_collision_handling, "test_unset_collision_handling");
}