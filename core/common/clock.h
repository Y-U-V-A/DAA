#ifndef CLOCK__H
#define CLOCK__H

#include "defines.h"

typedef struct clock {
    f64 start;
    f64 elapsed;
} clock;

void clock_set(clock* clk);

void clock_update(clock* clk);

#endif