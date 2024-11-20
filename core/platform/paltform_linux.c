#include "platform.h"

#ifdef ZLINUX

#    include <time.h>
#    include <unistd.h>
#    include <stdio.h>

void platform_console_write(i32 level, const char* msg, u64 size) {
    static const char* color_codes[5] = {
        "\033[31m", // Red (error)
        "\033[33m", // Yellow (warn)
        "\033[37m", // White (info)
        "\033[34m", // Blue (debug)
        "\033[32m"  // Green (trace)
    };

    fprintf(stderr, "%s%s\033[0m", color_codes[level], msg);
}

void platform_sleep(u64 ms) {
    usleep(ms * 1000); // Convert milliseconds to microseconds
}

f64 platform_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (ts.tv_nsec / 1.0e9);
}

#endif