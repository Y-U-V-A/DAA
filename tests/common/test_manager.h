#ifndef TEST_MANAGER_H
#define TEST_MANAGER_H

#include "defines.h"

typedef u32 (*PFN_test)();

void test_manager_init();

void test_manager_destroy();

void test_manager_register_test(PFN_test func, const char* msg);

void test_manager_run();

#endif