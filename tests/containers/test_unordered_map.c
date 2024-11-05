#include "test_unordered_map.h"
#include "unordered_map.h"
#include "expect.h"
#include "test_manager.h"

// Custom structure for testing
typedef struct {
    i32 id;
    f32 value;
} test_struct;

// Hash function for integers
u64 int_hash(const void* key, u64 key_stride) {
    return (*(i32*)key) % UNORDERED_MAP_DEFAULT_SIZE;
}

// Hash function for test_struct (using id field as key)
u64 struct_hash(const void* key, u64 key_stride) {
    return ((test_struct*)key)->id % UNORDERED_MAP_DEFAULT_SIZE;
}

// Test for i32 keys and values
u32 test_unordered_map_int() {
    unordered_map* map = unordered_map_create(i32, i32, int_hash);

    i32 key1 = 42, value1 = 100;
    i32 key2 = 23, value2 = 200;
    i32 key3 = 15, value3 = 300;

    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);
    unordered_map_insert(map, &key3, &value3);

    expect_should_be(value1, *(i32*)unordered_map_data(map, &key1));
    expect_should_be(value2, *(i32*)unordered_map_data(map, &key2));
    expect_should_be(value3, *(i32*)unordered_map_data(map, &key3));

    expect_should_be(3, unordered_map_size(map));

    unordered_map_remove(map, &key2);
    expect_should_be(2, unordered_map_size(map));

    expect_should_be(0, unordered_map_data(map, &key2));

    unordered_map_destroy(map);
    return true;
}

// Test for f32 keys and values
u32 test_unordered_map_float() {
    unordered_map* map = unordered_map_create(f32, f32, int_hash); // use int_hash for simplicity

    f32 key1 = 3.14f, value1 = 1.234f;
    f32 key2 = 2.71f, value2 = 2.345f;
    f32 key3 = 1.61f, value3 = 3.456f;

    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);
    unordered_map_insert(map, &key3, &value3);

    expect_float_should_be(value1, *(f32*)unordered_map_data(map, &key1));
    expect_float_should_be(value2, *(f32*)unordered_map_data(map, &key2));
    expect_float_should_be(value3, *(f32*)unordered_map_data(map, &key3));

    expect_should_be(3, unordered_map_size(map));

    unordered_map_destroy(map);

    return true;
}

// Test for struct keys and values
u32 test_unordered_map_struct() {
    unordered_map* map = unordered_map_create(test_struct, test_struct, 0);

    test_struct key1 = {1, 1.11f}, value1 = {10, 10.10f};
    test_struct key2 = {2, 2.22f}, value2 = {20, 20.20f};
    test_struct key3 = {3, 3.33f}, value3 = {30, 30.30f};

    unordered_map_insert(map, &key1, &value1);
    unordered_map_insert(map, &key2, &value2);
    unordered_map_insert(map, &key3, &value3);

    expect_should_be(value1.id, ((test_struct*)unordered_map_data(map, &key1))->id);
    expect_float_should_be(value1.value, ((test_struct*)unordered_map_data(map, &key1))->value);

    expect_should_be(value2.id, ((test_struct*)unordered_map_data(map, &key2))->id);
    expect_float_should_be(value2.value, ((test_struct*)unordered_map_data(map, &key2))->value);

    expect_should_be(value3.id, ((test_struct*)unordered_map_data(map, &key3))->id);
    expect_float_should_be(value3.value, ((test_struct*)unordered_map_data(map, &key3))->value);

    expect_should_be(3, unordered_map_size(map));

    unordered_map_destroy(map);

    return true;
}

void test_unordered_map_register() {

    test_manager_register_test(test_unordered_map_int, "test_unordered_map_int");
    test_manager_register_test(test_unordered_map_float, "test_unordered_map_float");
    test_manager_register_test(test_unordered_map_struct, "test_unordered_map_struct");
}