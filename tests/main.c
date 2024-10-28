#include "common/test_manager.h"
#include "containers/test_darray.h"
#include "containers/test_list.h"
#include "containers/test_stack.h"
#include "containers/test_queue.h"

#include "common.h"

int main() {

    test_manager_init();

    // register;
    test_darray_register();
    test_list_register();
    test_stack_register();
    test_queue_register();

    test_manager_run();

    test_manager_destroy();

    memory_state_log();
}