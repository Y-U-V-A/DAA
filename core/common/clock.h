#ifndef CLOCK_H
#define CLOCK_H

#include "defines.h"

typedef struct clock {
    f64 start;
    f64 elapsed;
} clock;

void clock_start(clock* clk);

void clock_update(clock* clk);

#endif