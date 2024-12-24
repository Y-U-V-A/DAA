#include "test_stack.h"
#include "expect.h"
#include "test_manager.h"
#include "stack.h"

// Test structures
typedef struct {
    i32 x;
    i32 y;
} Point;

typedef struct {
    char name[32];
    f32 score;
} Student;

// Test integer operations
u32 test_stack_int() {
    stack* stk = stack_create(i32);

    // Test initial state
    expect_should_be(0, stack_length(stk));
    expect_should_be(0, (u64)stack_top(stk));

    // Test push operations
    i32 values[] = {10, 20, 30, 40, 50};

    for (u32 i = 0; i < 5; i++) {
        stack_push(stk, &values[i]);
        expect_should_be(i + 1, stack_length(stk));
        expect_should_be(values[i], *(i32*)stack_top(stk));
    }

    // Test pop operations
    for (i32 i = 4; i >= 0; i--) {
        expect_should_be(values[i], *(i32*)stack_top(stk));
        stack_pop(stk);
        expect_should_be((u32)i, stack_length(stk));
    }

    stack_destroy(stk);
    return true;
}

// Test f32 operations
u32 test_stack_float() {
    stack* stk = stack_create(f32);

    f32 values[] = {1.5f, 2.7f, 3.14f, 4.2f};

    // Push and verify
    for (i32 i = 0; i < 4; i++) {
        stack_push(stk, &values[i]);
        expect_float_should_be(values[i], *(f32*)stack_top(stk), EPSILON);
    }

    // Pop and verify
    for (i32 i = 3; i >= 0; i--) {
        expect_float_should_be(values[i], *(f32*)stack_top(stk), EPSILON);
        stack_pop(stk);
    }

    stack_destroy(stk);
    return true;
}

// Test Point structure operations
u32 test_stack_point() {
    stack* stk = stack_create(Point);

    Point points[] = {
        {1, 2},
        {3, 4},
        {5, 6}};

    // Push and verify points
    for (i32 i = 0; i < 3; i++) {
        stack_push(stk, &points[i]);
        Point* top = (Point*)stack_top(stk);
        expect_should_be(points[i].x, top->x);
        expect_should_be(points[i].y, top->y);
    }

    // Pop and verify points
    for (i32 i = 2; i >= 0; i--) {
        Point* top = (Point*)stack_top(stk);
        expect_should_be(points[i].x, top->x);
        expect_should_be(points[i].y, top->y);
        stack_pop(stk);
    }

    stack_destroy(stk);
    return true;
}

// Test Student structure operations
u32 test_stack_student() {
    stack* stk = stack_create(Student);

    Student students[] = {
        {"Alice", 95.5f},
        {"Bob", 87.3f},
        {"Charlie", 91.8f}};

    // Push and verify students
    for (u32 i = 0; i < 3; i++) {
        stack_push(stk, &students[i]);
        Student* top = (Student*)stack_top(stk);
        expect_float_should_be(students[i].score, top->score, EPSILON);
        // Note: String comparison would need a separate function
        // For now we'll just verify the size
        expect_should_be(stack_length(stk), i + 1);
    }

    // Pop and verify students
    for (i32 i = 2; i >= 0; i--) {
        Student* top = (Student*)stack_top(stk);
        expect_float_should_be(students[i].score, top->score, EPSILON);
        stack_pop(stk);
    }

    stack_destroy(stk);
    return true;
}

void register_stack_tests() {

    test_manager_register_test(test_stack_int, "test_stack_int");
    test_manager_register_test(test_stack_float, "test_stack_float");
    test_manager_register_test(test_stack_point, "test_stack_point");
    test_manager_register_test(test_stack_student, "test_stack_student");
}