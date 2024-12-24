#include "test_unordered_map.h"
#include "unordered_map.h"
#include "expect.h"
#include "test_manager.h"

// Custom structure for testing
typedef struct test_struct {
    i32 id;
    f32 value;
    char name[32];
} test_struct;

u32 test_unmap_int_keys_and_values() {
    unordered_map* map = unordered_map_create(i32, i32, 0);

    // Test insertion
    i32 key1 = 5, value1 = 10;
    i32 key2 = 15, value2 = 20;
    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);

    // Test size
    expect_should_be(2, unordered_map_length(map));

    // Test retrieval
    i32* retrieved1 = (i32*)unordered_map_data(map, &key1);
    i32* retrieved2 = (i32*)unordered_map_data(map, &key2);
    expect_should_be(value1, *retrieved1);
    expect_should_be(value2, *retrieved2);

    // Test contains
    expect_should_be(true, unordered_map_contains(map, &key1));
    i32 nonexistent_key = 100;
    expect_should_be(false, unordered_map_contains(map, &nonexistent_key));

    // Test removal
    unordered_map_remove(map, &key1);
    expect_should_be(1, unordered_map_length(map));
    expect_should_be(false, unordered_map_contains(map, &key1));

    unordered_map_destroy(map);
    return true;
}

u32 test_unmap_float_keys_and_values() {
    unordered_map* map = unordered_map_create(f32, f32, 0);

    // Test insertion
    f32 key1 = 3.14f, value1 = 6.28f;
    f32 key2 = 2.718f, value2 = 5.436f;
    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);

    // Test size
    expect_should_be(2, unordered_map_length(map));

    // Test retrieval
    f32* retrieved1 = (f32*)unordered_map_data(map, &key1);
    f32* retrieved2 = (f32*)unordered_map_data(map, &key2);
    expect_float_should_be(value1, *retrieved1, EPSILON);
    expect_float_should_be(value2, *retrieved2, EPSILON);

    // Test update
    f32 new_value = 10.0f;
    unordered_map_insert(map, &key1, &new_value);
    f32* updated = (f32*)unordered_map_data(map, &key1);
    expect_float_should_be(new_value, *updated, EPSILON);

    unordered_map_destroy(map);
    return true;
}

u32 test_unmap_struct_keys_and_values() {
    unordered_map* map = unordered_map_create(test_struct, test_struct, 0);

    // Test insertion
    test_struct key1 = {1, 1.1f, "First"};
    test_struct value1 = {10, 10.1f, "Value1"};
    test_struct key2 = {2, 2.2f, "Second"};
    test_struct value2 = {20, 20.2f, "Value2"};

    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);

    // Test size
    expect_should_be(2, unordered_map_length(map));

    // Test retrieval
    test_struct* retrieved1 = (test_struct*)unordered_map_data(map, &key1);
    expect_should_be(value1.id, retrieved1->id);
    expect_float_should_be(value1.value, retrieved1->value, EPSILON);

    // Test collision handling
    test_struct key3 = {3, 3.3f, "Third"};
    test_struct value3 = {30, 30.3f, "Value3"};
    unordered_map_insert(map, &key3, &value3);

    test_struct* retrieved3 = (test_struct*)unordered_map_data(map, &key3);
    expect_should_be(value3.id, retrieved3->id);
    expect_float_should_be(value3.value, retrieved3->value, EPSILON);

    unordered_map_destroy(map);
    return true;
}

u32 test_unmap_resize_functionality() {
    unordered_map* map = unordered_map_create(i32, i32, 0);

    // Insert enough elements to trigger resize
    for (i32 i = 0; i < 30; i++) {
        i32 value = i * 10;
        unordered_map_insert(map, &i, &value);
    }

    // Verify all elements are still accessible after resize
    for (i32 i = 0; i < 30; i++) {
        i32* retrieved = (i32*)unordered_map_data(map, &i);
        expect_should_be(i * 10, *retrieved);
    }

    // Verify capacity has increased
    expect_should_be(true, unordered_map_capacity(map) > UNORDERED_MAP_DEFAULT_SIZE);

    unordered_map_destroy(map);
    return true;
}

void register_unordered_map_tests() {

    test_manager_register_test(test_unmap_int_keys_and_values, "test_unmap_int_keys_and_values");
    test_manager_register_test(test_unmap_float_keys_and_values, "test_unmap_float_keys_and_values");
    test_manager_register_test(test_unmap_struct_keys_and_values, "test_unmap_struct_keys_and_values");
    test_manager_register_test(test_unmap_resize_functionality, "test_unmap_resize_functionality");
}