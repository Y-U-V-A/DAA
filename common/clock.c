#include "clock.h"
#include "common.h"

void clock_start(clock* clk) {
    clk->start = get_time();
}

void clock_end(clock* clk) {
    clk->elapsed = get_time() - clk->start;
}