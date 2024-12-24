#include "test_map.h"
#include "expect.h"
#include "test_manager.h"
#include "map.h"
#include "zmemory.h"
#include "utils.h"

typedef struct {
    i32 id;
    f32 value;
    char name[32];
} TestStruct;

// Comparison functions for different types
bool map_int_compare(const void* a, const void* b) {
    return *(const i32*)a > *(const i32*)b;
}

bool map_float_compare(const void* a, const void* b) {
    return *(const f32*)a > *(const f32*)b;
}

bool map_struct_compare(const void* a, const void* b) {
    return ((const TestStruct*)a)->id > ((const TestStruct*)b)->id;
}

// Helper function to create a test struct
TestStruct map_create_test_struct(i32 id, f32 value, const char* name) {
    TestStruct s;
    s.id = id;
    s.value = value;
    string_copy(s.name, name, 31);
    s.name[31] = '\0';
    return s;
}

// Integer Map Tests
u32 test_map_int_basic_operations() {

    map* int_map = map_create(i32, i32, map_int_compare);

    // Test insertion
    i32 key1 = 1, value1 = 100;
    i32 key2 = 2, value2 = 200;
    i32 key3 = 3, value3 = 300;

    map_insert(int_map, &key1, &value1);
    map_insert(int_map, &key2, &value2);
    map_insert(int_map, &key3, &value3);

    expect_should_be(3, map_length(int_map));

    // Test retrieval
    i32* retrieved = (i32*)map_data(int_map, &key2);
    expect_should_be(200, *retrieved);

    // Test contains
    expect_should_be(true, map_contains(int_map, &key1));
    expect_should_be(false, map_contains(int_map, &(i32){4}));

    // Test removal
    map_remove(int_map, &key2);
    expect_should_be(2, map_length(int_map));
    expect_should_be(false, map_contains(int_map, &key2));

    map_destroy(int_map);
    return true;
}

// Float Map Tests
u32 test_map_float_basic_operations() {
    map* float_map = map_create(f32, f32, map_float_compare);

    // Test insertion
    f32 key1 = 1.5f, value1 = 10.5f;
    f32 key2 = 2.5f, value2 = 20.5f;
    f32 key3 = 3.5f, value3 = 30.5f;

    map_insert(float_map, &key1, &value1);
    map_insert(float_map, &key2, &value2);
    map_insert(float_map, &key3, &value3);

    expect_should_be(3, map_length(float_map));

    // Test retrieval
    f32* retrieved = (f32*)map_data(float_map, &key2);
    expect_float_should_be(20.5f, *retrieved, EPSILON);

    // Test update
    f32 new_value = 25.5f;
    map_insert(float_map, &key2, &new_value);
    retrieved = (f32*)map_data(float_map, &key2);
    expect_float_should_be(25.5f, *retrieved, EPSILON);

    map_destroy(float_map);
    return true;
}

// Struct Map Tests
u32 test_map_struct_basic_operations() {
    map* struct_map = map_create(TestStruct, TestStruct, map_struct_compare);

    // Create test structs
    TestStruct s1 = map_create_test_struct(1, 10.5f, "First");
    TestStruct s2 = map_create_test_struct(2, 20.5f, "Second");
    TestStruct s3 = map_create_test_struct(3, 30.5f, "Third");

    // Test insertion
    map_insert(struct_map, &s1, &s1);
    map_insert(struct_map, &s2, &s2);
    map_insert(struct_map, &s3, &s3);

    expect_should_be(3, map_length(struct_map));

    // Test retrieval
    TestStruct* retrieved = (TestStruct*)map_data(struct_map, &s2);
    expect_should_be(2, retrieved->id);
    expect_float_should_be(20.5f, retrieved->value, EPSILON);

    // Test iteration
    map_node* node = map_begin(struct_map);
    expect_should_be(1, ((TestStruct*)node->key)->id);

    node = map_next(struct_map, node);
    expect_should_be(2, ((TestStruct*)node->key)->id);

    node = map_next(struct_map, node);
    expect_should_be(3, ((TestStruct*)node->key)->id);

    map_destroy(struct_map);
    return true;
}

// Edge Cases Tests
u32 test_map_edge_cases() {
    map* int_map = map_create(i32, i32, map_int_compare);

    // Test empty map operations
    expect_should_be(0, map_length(int_map));
    expect_should_be(false, map_contains(int_map, &(i32){1}));
    expect_should_be(0, map_data(int_map, &(i32){1}));

    // Test duplicate keys
    i32 key = 1, value1 = 100, value2 = 200;
    map_insert(int_map, &key, &value1);
    map_insert(int_map, &key, &value2);
    expect_should_be(1, map_length(int_map));
    expect_should_be(200, *(i32*)map_data(int_map, &key));

    // Test removal of non-existent key
    map_remove(int_map, &(i32){999});
    expect_should_be(1, map_length(int_map));

    map_destroy(int_map);
    return true;
}

void register_map_tests() {
    test_manager_register_test(test_map_int_basic_operations, "test_map_int_basic_operations");
    test_manager_register_test(test_map_float_basic_operations, "test_map_float_basic_operations");
    test_manager_register_test(test_map_struct_basic_operations, "test_map_struct_basic_operations");
    test_manager_register_test(test_map_edge_cases, "test_map_edge_cases");
}