#include "test_manager.h"
#include "test_darray.h"
#include "test_list.h"
#include "test_stack.h"
#include "test_queue.h"
#include "test_priority_queue.h"
#include "test_unordered_map.h"
#include "test_map.h"
#include "test_unordered_set.h"
#include "test_set.h"
#include "testing_linear_allocator.h"
#include "testing_freelist_allocator.h"

#include "logger.h"
#include "zmemory.h"

// ** TESTS ARE GENERATED USING AI **

int main() {

    zmemory_init();
    zmemory_log();

    logger_init(1024 * 1024); // 1mb

    // init
    test_manager_init();

    // register;
    register_darray_tests();
    register_list_tests();
    register_stack_tests();
    register_queue_tests();
    register_priority_queue_tests();
    register_map_tests();
    register_unordered_map_tests();
    register_set_tests();
    register_unordered_set_tests();
    register_linear_tests();
    register_freelist_tests();

    // run
    test_manager_run();

    // destroy
    test_manager_destroy();

    logger_shutdown();

    zmemory_log();
    zmemory_destroy();
}