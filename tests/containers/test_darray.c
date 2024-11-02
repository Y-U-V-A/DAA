#include "test_darray.h"
#include "test_manager.h"
#include "expect.h"
#include "darray.h"

#include "common.h"

typedef struct {
    i32 x;
    float y;
    char name[32];
} TestStruct;

u32 test_darray_init() {

    i32* array = darray_create(i32);

    expect_should_be(DARRAY_DEFAULT_LENGTH, darray_capacity(array));
    expect_should_be(sizeof(i32), darray_stride(array));
    expect_should_be(0, darray_length(array));

    darray_destroy(array);
    return true;
}

u32 test_darray_push_back_integers() {
    i32* array = darray_create(i32);

    // Test pushing elements
    i32 test_value = 42;
    darray_push_back(array, test_value);
    expect_should_be(1, darray_length(array));
    expect_should_be(test_value, array[0]);

    // Test array resizing
    for (i32 i = 0; i < 5; i++) {
        darray_push_back(array, i);
    }

    expect_should_be(6, darray_length(array));
    expect_should_be(8, darray_capacity(array)); // Should have resized to 8 (2 * DARRAY_DEFAULT_LENGTH * DARRAY_RESIZE_FACTOR)

    darray_destroy(array);
    return true;
}

u32 test_darray_pop_back_integers() {
    i32* array = darray_create(i32);

    // Push some elements
    for (i32 i = 0; i < 3; i++) {
        darray_push_back(array, i);
    }

    expect_should_be(3, darray_length(array));

    // Test popping
    darray_pop_back(array);
    expect_should_be(2, darray_length(array));
    expect_should_be(1, array[1]);

    darray_destroy(array);
    return true;
}

u32 test_darray_insert_integers() {
    i32* array = darray_create(i32);

    // Push initial elements
    for (i32 i = 0; i < 3; i++) {
        darray_push_back(array, i);
    }

    // Test insertion
    i32 insert_value = 42;
    darray_insert(array, 1, insert_value);

    expect_should_be(4, darray_length(array));
    expect_should_be(insert_value, array[1]);
    expect_should_be(1, array[2]); // Original value should have been shifted

    darray_destroy(array);
    return true;
}

u32 test_darray_remove_integers() {
    i32* array = darray_create(i32);

    // Push initial elements
    for (i32 i = 0; i < 4; i++) {
        darray_push_back(array, i);
    }

    // Test removal
    darray_remove(array, 1);

    expect_should_be(3, darray_length(array));
    expect_should_be(2, array[1]); // Element at index 2 should have shifted down

    darray_destroy(array);
    return true;
}

u32 test_darray_struct_operations() {
    TestStruct* array = darray_create(TestStruct);

    // Test pushing structs
    TestStruct test_item = {
        .x = 42,
        .y = 3.14f,
        .name = "Test1",
    };

    darray_push_back(array, test_item);

    expect_should_be(1, darray_length(array));
    expect_should_be(test_item.x, array[0].x);
    expect_float_should_be(test_item.y, array[0].y);

    // Test inserting structs
    TestStruct insert_item = {
        .x = 100,
        .y = 2.718f,
        .name = "Test2",
    };

    darray_insert(array, 0, insert_item);

    expect_should_be(2, darray_length(array));
    expect_should_be(insert_item.x, array[0].x);
    expect_should_be(test_item.x, array[1].x);

    darray_destroy(array);
    return true;
}

void test_darray_register() {

    test_manager_register_test(test_darray_init, "test_darray_init");
    test_manager_register_test(test_darray_push_back_integers, "test_darray_push_back_integers");
    test_manager_register_test(test_darray_pop_back_integers, "test_darray_pop_back_integers");
    test_manager_register_test(test_darray_insert_integers, "test_darray_insert_integers");
    test_manager_register_test(test_darray_remove_integers, "test_darray_remove_integers");
    test_manager_register_test(test_darray_struct_operations, "test_darray_struct_operations");
}