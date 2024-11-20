#ifndef PLATFORM__H
#define PLATFORM__H

#include "defines.h"

void platform_console_write(i32 level, const char* msg, u64 size);

f64 platform_time();

void platform_sleep(u64 ms);

#endif