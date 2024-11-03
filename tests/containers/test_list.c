#include "test_list.h"
#include "expect.h"
#include "test_manager.h"
#include "list.h"

// Test struct
typedef struct {
    int id;
    float score;
    char name[32];
} Student;

// Comparison functions
b8 compare_ints(const void* a, const void* b) {
    return *(const int*)a <= *(const int*)b;
}

b8 unique_ints(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

b8 compare_students_by_id(const void* a, const void* b) {
    const Student* s1 = (const Student*)a;
    const Student* s2 = (const Student*)b;
    return s1->id <= s2->id;
}

u32 test_list_init() {
    list* lst = list_create(int);

    expect_should_be(0, list_size(lst));

    list_destroy(lst);
    return true;
}

u32 test_list_push_back_ints() {
    list* lst = list_create(int);

    // Test pushing elements
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        list_push_back(lst, &values[i]);
    }

    expect_should_be(5, list_size(lst));

    // Verify values in order
    for (int i = 0; i < 5; i++) {
        int* value = (int*)list_data(lst, i);
        expect_should_be(values[i], *value);
    }

    list_destroy(lst);
    return true;
}

u32 test_list_push_front_ints() {
    list* lst = list_create(int);

    int values[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        list_push_front(lst, &values[i]);
    }

    expect_should_be(3, list_size(lst));

    // Values should be in reverse order due to push_front
    int* first = (int*)list_data(lst, 0);
    expect_should_be(3, *first);

    list_destroy(lst);
    return true;
}

u32 test_list_pop_operations() {
    list* lst = list_create(int);

    int values[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        list_push_back(lst, &values[i]);
    }

    // Test pop_back
    list_pop_back(lst);
    expect_should_be(3, list_size(lst));

    int* last = (int*)list_data(lst, 2);
    expect_should_be(3, *last);

    // Test pop_front
    list_pop_front(lst);
    expect_should_be(2, list_size(lst));

    int* new_first = (int*)list_data(lst, 0);
    expect_should_be(2, *new_first);

    list_destroy(lst);
    return true;
}

u32 test_list_insert_remove() {
    list* lst = list_create(int);

    int values[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        list_push_back(lst, &values[i]);
    }

    // Test insert
    int insert_val = 42;
    list_insert(lst, 1, &insert_val);
    expect_should_be(4, list_size(lst));

    int* inserted = (int*)list_data(lst, 1);
    expect_should_be(42, *inserted);

    // Test remove
    list_remove(lst, 1);
    expect_should_be(3, list_size(lst));

    int* after_remove = (int*)list_data(lst, 1);
    expect_should_be(2, *after_remove);

    list_destroy(lst);
    return true;
}

u32 test_list_sort_ints() {
    list* lst = list_create(int);

    int values[] = {5, 2, 8, 1, 9};
    for (int i = 0; i < 5; i++) {
        list_push_back(lst, &values[i]);
    }

    list_sort(lst, compare_ints);

    // Verify sorted order
    int prev_val = -1;
    for (int i = 0; i < 5; i++) {
        int* curr = (int*)list_data(lst, i);
        expect_should_be_true(*curr >= prev_val);
        prev_val = *curr;
    }

    list_destroy(lst);
    return true;
}

u32 test_list_reverse_ints() {
    list* lst = list_create(int);

    int values[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        list_push_back(lst, &values[i]);
    }

    list_reverse(lst);

    // Verify reversed order
    for (int i = 0; i < 4; i++) {
        int* value = (int*)list_data(lst, i);
        expect_should_be(values[3 - i], *value);
    }

    list_destroy(lst);
    return true;
}

u32 test_list_unique_ints() {
    list* lst = list_create(int);

    // Add duplicates
    int values[] = {1, 1, 2, 2, 3, 3, 3};
    for (int i = 0; i < 7; i++) {
        list_push_back(lst, &values[i]);
    }

    list_sort(lst, compare_ints);
    list_unique(lst, unique_ints);

    expect_should_be(3, list_size(lst));

    int* first = (int*)list_data(lst, 0);
    int* second = (int*)list_data(lst, 1);
    int* third = (int*)list_data(lst, 2);

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

    expect_should_be(2, list_size(lst));

    // Test struct data retrieval
    Student* retrieved = (Student*)list_data(lst, 0);
    expect_should_be(1, retrieved->id);
    expect_float_should_be(85.5f, retrieved->score);

    // Test sorting structs
    list_sort(lst, compare_students_by_id);

    Student* first = (Student*)list_data(lst, 0);
    Student* second = (Student*)list_data(lst, 1);
    expect_should_be(1, first->id);
    expect_should_be(2, second->id);

    list_destroy(lst);
    return true;
}

u32 test_list_merge_operations() {
    list* lst1 = list_create(int);
    list* lst2 = list_create(int);

    // Create two sorted lists
    int values1[] = {1, 3, 5};
    int values2[] = {2, 4, 6};

    for (int i = 0; i < 3; i++) {
        list_push_back(lst1, &values1[i]);
        list_push_back(lst2, &values2[i]);
    }

    list_merge(lst1, lst2, compare_ints);

    expect_should_be(6, list_size(lst1));

    // Verify merged list is sorted
    int prev_val = -1;
    for (int i = 0; i < 6; i++) {
        int* curr = (int*)list_data(lst1, i);
        expect_should_be_true(*curr >= prev_val);
        prev_val = *curr;
    }

    list_destroy(lst1);
    list_destroy(lst2);

    return true;
}

void test_list_register() {

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
