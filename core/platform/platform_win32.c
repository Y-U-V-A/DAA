#include "platform.h"

#ifdef ZWINDOWS

#    include <Windows.h>
#    include "logger.h"
#    include "zthread.h"
#    include "zmutex.h"
#    include "zsemaphore.h"

void platform_console_write(i32 level, const char* msg, u64 size) {

    static u8 color_codes[5] = {
        12, // Red (error)
        14, // Yellow (warn)
        15, // White (info)
        11, // Blue (debug)
        10, // Green (trace)
    };

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, color_codes[level]);

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

////////////////////////////////////////////////////////////////////////////
//              __      __                                            __  //
//             /  |    /  |                                          /  | //
//  ________  _$$ |_   $$ |____    ______    ______    ______    ____$$ | //
// /        |/ $$   |  $$      \  /      \  /      \  /      \  /    $$ | //
// $$$$$$$$/ $$$$$$/   $$$$$$$  |/$$$$$$  |/$$$$$$  | $$$$$$  |/$$$$$$$ | //
//   /  $$/    $$ | __ $$ |  $$ |$$ |  $$/ $$    $$ | /    $$ |$$ |  $$ | //
//  /$$$$/__   $$ |/  |$$ |  $$ |$$ |      $$$$$$$$/ /$$$$$$$ |$$ \__$$ | //
// /$$      |  $$  $$/ $$ |  $$ |$$ |      $$       |$$    $$ |$$    $$ | //
// $$$$$$$$/    $$$$/  $$/   $$/ $$/        $$$$$$$/  $$$$$$$/  $$$$$$$/  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

bool zthread_create(PFN_zthread_start func, void* params, zthread* out_thread) {
    if (!func || !out_thread) {
        LOGE("zthread_create : invalid params");
        return false;
    }
    out_thread->internal_data = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)func, params, 0, 0);
    if (!out_thread->internal_data) {
        LOGE("zthread_create : failed to create zthread");
        return false;
    }
    return true;
}

void zthread_destroy(zthread* thread) {
    if (!thread) {
        LOGE("zthread_destroy : invalid params");
        return;
    }

    CloseHandle(thread->internal_data);
}

bool zthread_wait(zthread* thread) {
    if (!thread) {
        LOGE("zthread_wait : invalid params");
        return false;
    }

    DWORD exit_code = WaitForSingleObject(thread->internal_data, INFINITE);

    if (exit_code == WAIT_TIMEOUT || exit_code == WAIT_FAILED) {
        LOGW("zthread_wait: timed out");
        return false;
    }

    return true;
}

bool zthread_wait_on_all(zthread* threads, u64 count) {
    if (!threads) {
        LOGE("zthread_wait_on_all : invalid params");
        return false;
    }

    DWORD exit_code = WaitForMultipleObjects(count, (void*)threads, 1, INFINITE);

    if (exit_code == WAIT_TIMEOUT || exit_code == WAIT_FAILED) {
        LOGW("zthread_wait_on_all: timed out");
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
//                                      __                          //
//                                     /  |                         //
//  ________  _____  ____   __    __  _$$ |_     ______   __    __  //
// /        |/     \/    \ /  |  /  |/ $$   |   /      \ /  \  /  | //
// $$$$$$$$/ $$$$$$ $$$$  |$$ |  $$ |$$$$$$/   /$$$$$$  |$$  \/$$/  //
//   /  $$/  $$ | $$ | $$ |$$ |  $$ |  $$ | __ $$    $$ | $$  $$<   //
//  /$$$$/__ $$ | $$ | $$ |$$ \__$$ |  $$ |/  |$$$$$$$$/  /$$$$  \  //
// /$$      |$$ | $$ | $$ |$$    $$/   $$  $$/ $$       |/$$/ $$  | //
// $$$$$$$$/ $$/  $$/  $$/  $$$$$$/     $$$$/   $$$$$$$/ $$/   $$/  //
//                                                                  //
//////////////////////////////////////////////////////////////////////

bool zmutex_create(zmutex* out_mutex) {
    if (!out_mutex) {
        LOGE("zmutex_create : invalid params");
        return false;
    }
    out_mutex->internal_data = CreateMutex(0, 0, 0);
    if (!out_mutex->internal_data) {
        LOGE("zmutex_create : failed to create zmutex");
        return false;
    }
    return true;
}

void zmutex_destroy(zmutex* mutex) {
    if (!mutex) {
        LOGE("zmutex_destroy : invalid params");
        return;
    }
    CloseHandle(mutex->internal_data);
}

bool zmutex_lock(zmutex* mutex) {

    if (!mutex) {
        LOGE("zmutex_lock : invalid params");
        return false;
    }
    // if mutex==false then mutex=true and continue executing thread
    // else blocks the thread
    DWORD exit_code = WaitForSingleObject(mutex->internal_data, INFINITE);

    if (exit_code == WAIT_TIMEOUT || exit_code == WAIT_FAILED) {
        LOGE("zmutex_lock : timed out");
        return false;
    }

    return true;
}

bool zmutex_unlock(zmutex* mutex) {
    if (!mutex) {
        LOGE("zmutex_unlock : invalid params");
        return false;
    }

    // if mutex==true then mutex=false
    // else undefined error
    if (!ReleaseMutex(mutex->internal_data)) {
        LOGE("zmutex_unlock : failed to unlock zmutex");
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                  __                                      //
//                                                                 /  |                                     //
//  ________   _______   ______   _____  ____    ______    ______  $$ |____    ______    ______    ______   //
// /        | /       | /      \ /     \/    \  /      \  /      \ $$      \  /      \  /      \  /      \  //
// $$$$$$$$/ /$$$$$$$/ /$$$$$$  |$$$$$$ $$$$  | $$$$$$  |/$$$$$$  |$$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$  | //
//   /  $$/  $$      \ $$    $$ |$$ | $$ | $$ | /    $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$/ $$    $$ | //
//  /$$$$/__  $$$$$$  |$$$$$$$$/ $$ | $$ | $$ |/$$$$$$$ |$$ |__$$ |$$ |  $$ |$$ \__$$ |$$ |      $$$$$$$$/  //
// /$$      |/     $$/ $$       |$$ | $$ | $$ |$$    $$ |$$    $$/ $$ |  $$ |$$    $$/ $$ |      $$       | //
// $$$$$$$$/ $$$$$$$/   $$$$$$$/ $$/  $$/  $$/  $$$$$$$/ $$$$$$$/  $$/   $$/  $$$$$$/  $$/        $$$$$$$/  //
//                                                       $$ |                                               //
//                                                       $$ |                                               //
//                                                       $$/                                                //
//                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool zsemaphore_create(u32 max_count, zsemaphore* out_semaphore) {
    if (!max_count || !out_semaphore) {
        LOGE("zsemaphore_create : invalid params");
        return false;
    }
    // create semaphore
    out_semaphore->internal_data = CreateSemaphore(0, max_count, max_count, 0);

    if (!out_semaphore->internal_data) {
        LOGE("zsemaphore_create : failed to create zsemaphore");
        return false;
    }

    return true;
}

void zsemaphore_destroy(zsemaphore* semaphore) {
    if (!semaphore) {
        LOGE("zsemaphore_destroy : invalid params");
        return;
    }
    CloseHandle(semaphore->internal_data);
}

bool zsemaphore_signal(zsemaphore* semaphore) {
    if (!semaphore) {
        LOGE("zsemaphore_signal : invalid params");
        return false;
    }

    /*
     lpPreviousCount
    A pointer to a variable to receive the previous count for the semaphore.
     This parameter can be 0 if the previous count is not required.
    */
    // this will increment the semaphore count;
    if (!ReleaseSemaphore(semaphore->internal_data, 1, 0)) {
        LOGE("zsemaphore_signal : failed to signal zsemaphore");
        return false;
    }

    return true;
}

bool zsemaphore_wait(zsemaphore* semaphore) {
    if (!semaphore) {
        LOGE("zsemaphore_wait : invalid params");
        return false;
    }

    // if semaphore_count>0 then semaphore_count-=1 and continue executing thread
    // else blocks the thread
    DWORD exit_code = WaitForSingleObject(semaphore->internal_data, INFINITE);

    if (exit_code == WAIT_TIMEOUT || exit_code == WAIT_FAILED) {
        LOGE("zsemaphore_wait : timed out");
        return false;
    }

    return true;
}

#endif