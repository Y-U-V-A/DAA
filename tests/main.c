#include "containers/test_darray.h"
#include "common/test_manager.h"

#include "common.h"

int main() {

    test_manager_init();

    // register;
    test_darray_reigster();

    test_manager_run();

    test_manager_destroy();

    memory_state_log();
}