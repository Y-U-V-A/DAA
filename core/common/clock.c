#include "clock.h"
#include "platform.h"

void clock_set(clock* clk) {
    clk->start = platform_time();
}

void clock_update(clock* clk) {
    clk->elapsed = platform_time() - clk->start;
}
