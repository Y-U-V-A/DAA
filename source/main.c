#include "algorithms/sorting/merge.h"
#include "logger.h"
#include "common.h"

int main() {

    logger_init(1024 * 1024); // 1mb

    memory_state_log();

    merge_run();

    memory_state_log();

    logger_shutdown();
}