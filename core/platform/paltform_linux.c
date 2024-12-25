#include "platform.h"

#ifdef ZLINUX

#    include <pthread.h>
#    include <unistd.h>
#    include <stdlib.h>
#    include <time.h>
#    include <stdio.h>
#    include <semaphore.h>
#    include "logger.h"
#    include "zthread.h"
#    include "zmutex.h"
#    include "zsemaphore.h"
// use - lrt(real time library) while linking

void platform_console_write(i32 level, const char* msg, u64 size) {

    static const char* color_codes[5] = {
        "\033[31m", // Red (error)
        "\033[33m", // Yellow (warn)
        "\033[37m", // White (info)
        "\033[34m", // Blue (debug)
        "\033[32m"  // Green (trace)
    };

    printf("%s%s\033[0m", color_codes[level], msg);
}

void platform_sleep(u64 ms) {
    usleep(ms * 1000); // Convert milliseconds to microseconds
}

f64 platform_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (ts.tv_nsec / 1.0e9);
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

    if (pthread_create((pthread_t*)out_thread, 0, func, params) != 0) {
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
}

bool zthread_wait(zthread* thread) {
    if (!thread) {
        LOGE("zthread_wait : invalid params");
        return false;
    }

    if (pthread_join((pthread_t)thread->internal_data, 0) != 0) {
        LOGW("zthread_wait: failed to join");
        return false;
    }

    return true;
}

bool zthread_wait_on_all(zthread* threads, u64 count) {
    if (!threads) {
        LOGE("zthread_wait_on_all : invalid params");
        return false;
    }

    for (u32 i = 0; i < count; ++i) {
        if (pthread_join((pthread_t)threads[i].internal_data, 0) != 0) {
            LOGW("zthread_wait_on_all: failed to join");
            return false;
        }
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
    out_mutex->internal_data = malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(out_mutex->internal_data, 0) != 0) {
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

    if (pthread_mutex_destroy(mutex->internal_data) != 0) {
        LOGE("zmutex_destroy : failed to destroy zmutex");
        return;
    }
    free(mutex->internal_data);
}

bool zmutex_lock(zmutex* mutex) {

    if (!mutex) {
        LOGE("zmutex_lock : invalid params");
        return false;
    }
    // if mutex==false then mutex=true and continue executing thread
    // else blocks the thread

    if (pthread_mutex_lock(mutex->internal_data) != 0) {
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
    if (pthread_mutex_unlock(mutex->internal_data) != 0) {
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
    out_semaphore->internal_data = malloc(sizeof(sem_t));
    if (sem_init(out_semaphore->internal_data, 0, max_count) != 0) {
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
    sem_destroy(semaphore->internal_data);
    free(semaphore->internal_data);
}

bool zsemaphore_signal(zsemaphore* semaphore) {
    if (!semaphore) {
        LOGE("zsemaphore_signal : invalid params");
        return false;
    }

    // this will increment the semaphore count;
    if (sem_post(semaphore->internal_data) != 0) {
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

    if (sem_wait(semaphore->internal_data) != 0) {
        LOGE("zsemaphore_wait : timed out");
        return false;
    }

    return true;
}

#endif