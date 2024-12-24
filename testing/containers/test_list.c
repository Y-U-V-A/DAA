#include "test_list.h"
#include "expect.h"
#include "test_manager.h"
#include "list.h"

// Test struct
typedef struct {
    i32 id;
    f32 score;
    char name[32];
} Student;

// Comparison functions
bool list_compare_ints(const void* a, const void* b) {
    return *(const i32*)a <= *(const i32*)b;
}

bool list_unique_ints(const void* a, const void* b) {
    return *(const i32*)a == *(const i32*)b;
}

bool list_compare_students_by_id(const void* a, const void* b) {
    const Student* s1 = (const Student*)a;
    const Student* s2 = (const Student*)b;
    return s1->id <= s2->id;
}

u32 test_list_init() {
    list* lst = list_create(i32);

    expect_should_be(0, list_length(lst));

    list_destroy(lst);
    return true;
}

u32 test_list_push_back_ints() {
    list* lst = list_create(i32);

    // Test pushing elements
    i32 values[] = {1, 2, 3, 4, 5};
    for (i32 i = 0; i < 5; i++) {
        list_push_back(lst, &values[i]);
    }

    expect_should_be(5, list_length(lst));

    // Verify values in order
    for (i32 i = 0; i < 5; i++) {
        i32* value = (i32*)list_data(lst, i);
        expect_should_be(values[i], *value);
    }

    list_destroy(lst);
    return true;
}

u32 test_list_push_front_ints() {
    list* lst = list_create(i32);

    i32 values[] = {1, 2, 3};
    for (i32 i = 0; i < 3; i++) {
        list_push_front(lst, &values[i]);
    }

    expect_should_be(3, list_length(lst));

    // Values should be in reverse order due to push_front
    i32* first = (i32*)list_data(lst, 0);
    expect_should_be(3, *first);

    list_destroy(lst);
    return true;
}

u32 test_list_pop_operations() {
    list* lst = list_create(i32);

    i32 values[] = {1, 2, 3, 4};
    for (i32 i = 0; i < 4; i++) {
        list_push_back(lst, &values[i]);
    }

    // Test pop_back
    list_pop_back(lst);
    expect_should_be(3, list_length(lst));

    i32* last = (i32*)list_data(lst, 2);
    expect_should_be(3, *last);

    // Test pop_front
    list_pop_front(lst);
    expect_should_be(2, list_length(lst));

    i32* new_first = (i32*)list_data(lst, 0);
    expect_should_be(2, *new_first);

    list_destroy(lst);
    return true;
}

u32 test_list_insert_remove() {
    list* lst = list_create(i32);

    i32 values[] = {1, 2, 3};
    for (i32 i = 0; i < 3; i++) {
        list_push_back(lst, &values[i]);
    }

    // Test insert
    i32 insert_val = 42;
    list_insert(lst, 1, &insert_val);
    expect_should_be(4, list_length(lst));

    i32* inserted = (i32*)list_data(lst, 1);
    expect_should_be(42, *inserted);

    // Test remove
    list_remove(lst, 1);
    expect_should_be(3, list_length(lst));

    i32* after_remove = (i32*)list_data(lst, 1);
    expect_should_be(2, *after_remove);

    list_destroy(lst);
    return true;
}

u32 test_list_sort_ints() {
    list* lst = list_create(i32);

    i32 values[] = {5, 2, 8, 1, 9};
    for (i32 i = 0; i < 5; i++) {
        list_push_back(lst, &values[i]);
    }

    list_sort(lst, list_compare_ints);

    // Verify sorted order
    i32 prev_val = -1;
    for (i32 i = 0; i < 5; i++) {
        i32* curr = (i32*)list_data(lst, i);
        expect_should_be(true, (*curr >= prev_val));
        prev_val = *curr;
    }

    list_destroy(lst);
    return true;
}

u32 test_list_reverse_ints() {
    list* lst = list_create(i32);

    i32 values[] = {1, 2, 3, 4};
    for (i32 i = 0; i < 4; i++) {
        list_push_back(lst, &values[i]);
    }

    list_reverse(lst);

    // Verify reversed order
    for (i32 i = 0; i < 4; i++) {
        i32* value = (i32*)list_data(lst, i);
        expect_should_be(values[3 - i], *value);
    }

    list_destroy(lst);
    return true;
}

u32 test_list_unique_ints() {
    list* lst = list_create(i32);

    // Add duplicates
    i32 values[] = {1, 1, 2, 2, 3, 3, 3};
    for (i32 i = 0; i < 7; i++) {
        list_push_back(lst, &values[i]);
    }

    list_sort(lst, list_compare_ints);
    list_unique(lst, list_unique_ints);

    expect_should_be(3, list_length(lst));

    i32* first = (i32*)list_data(lst, 0);
    i32* second = (i32*)list_data(lst, 1);
    i32* third = (i32*)list_data(lst, 2);

    expect_should_be(1, *first);
    expect_should_be(2, *second);
    expect_should_be(3, *third);

    list_destroy(lst);
    return true;
}

u32 test_list_struct_operations() {
    list* lst = list_create(Student);

    // Test pushing structs
    Student s1 = {.id = 1, .score = 85.5f, .name = "Alice"};

    Student s2 = {.id = 2, .score = 92.0f, .name = "Bob"};

    list_push_back(lst, &s1);
    list_push_back(lst, &s2);

    expect_should_be(2, list_length(lst));

    // Test struct data retrieval
    Student* retrieved = (Student*)list_data(lst, 0);
    expect_should_be(1, retrieved->id);
    expect_float_should_be(85.5f, retrieved->score, EPSILON);

    // Test sorting structs
    list_sort(lst, list_compare_students_by_id);

    Student* first = (Student*)list_data(lst, 0);
    Student* second = (Student*)list_data(lst, 1);
    expect_should_be(1, first->id);
    expect_should_be(2, second->id);

    list_destroy(lst);
    return true;
}

u32 test_list_merge_operations() {
    list* lst1 = list_create(i32);
    list* lst2 = list_create(i32);

    // Create two sorted lists
    i32 values1[] = {1, 3, 5};
    i32 values2[] = {2, 4, 6};

    for (i32 i = 0; i < 3; i++) {
        list_push_back(lst1, &values1[i]);
        list_push_back(lst2, &values2[i]);
    }

    list_merge(lst1, lst2, list_compare_ints);

    expect_should_be(6, list_length(lst1));

    // Verify merged list is sorted
    i32 prev_val = -1;
    for (i32 i = 0; i < 6; i++) {
        i32* curr = (i32*)list_data(lst1, i);
        expect_should_be(true, (*curr >= prev_val));
        prev_val = *curr;
    }

    list_destroy(lst1);
    list_destroy(lst2);

    return true;
}

void register_list_tests() {

    test_manager_register_test(test_list_init, "test_list_init");
    test_manager_register_test(test_list_push_back_ints, "test_list_push_back_ints");
    test_manager_register_test(test_list_push_front_ints, "test_list_push_front_ints");
    test_manager_register_test(test_list_pop_operations, "test_list_pop_operations");
    test_manager_register_test(test_list_insert_remove, "test_list_insert_remove");
    test_manager_register_test(test_list_sort_ints, "test_list_sort_ints");
    test_manager_register_test(test_list_reverse_ints, "test_list_reverse_ints");
    test_manager_register_test(test_list_unique_ints, "test_list_unique_ints");
    test_manager_register_test(test_list_struct_operations, "test_list_struct_operations");
    test_manager_register_test(test_list_merge_operations, "test_list_merge_operations");
}
