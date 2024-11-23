#include "common/test_manager.h"
#include "containers/test_darray.h"
#include "containers/test_list.h"
#include "containers/test_stack.h"
#include "containers/test_queue.h"
#include "containers/test_priority_queue.h"
#include "containers/test_unordered_map.h"
#include "containers/test_map.h"
#include "containers/test_unordered_set.h"
#include "containers/test_set.h"

#include "logger.h"
#include "common.h"

// ** TESTS ARE GENERATED USING AI **

int main() {

    logger_init(1024 * 1024); // 1mb

    memory_state_log();

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

    // run
    test_manager_run();

    // destroy
    test_manager_destroy();

    // log mem state
    memory_state_log();

    logger_shutdown();
}