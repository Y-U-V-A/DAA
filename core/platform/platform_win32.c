#include "platform.h"

#ifdef ZWINDOWS

#    include <Windows.h>

void platform_console_write(i32 level, const char* msg, u64 size) {

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    static u8 levels[5] = {12, 14, 15, 11, 10};
    SetConsoleTextAttribute(console, levels[level]);

    WriteConsoleA(console, msg, size, 0, 0);

    SetConsoleTextAttribute(console, 7);
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

f64 platform_time() {
    LARGE_INTEGER curr;
    QueryPerformanceCounter(&curr);
    LARGE_INTEGER frq;
    QueryPerformanceFrequency(&frq);
    return curr.QuadPart / (double)frq.QuadPart;
}

#endif