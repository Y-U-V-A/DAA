#ifndef ZTHREAD__H
#define ZTHREAD__H

#include "defines.h"

typedef struct zthread {
    void* internal_data;
} zthread;

#ifdef ZWINDOWS
typedef u32 (*PFN_zthread_start)(void*);
#endif
#ifdef ZLINUX
typedef void* (*PFN_zthread_start)(void*);
#endif

bool zthread_create(PFN_zthread_start func, void* params, zthread* out_thread);

void zthread_destroy(zthread* thread);

bool zthread_wait(zthread* thread);

bool zthread_wait_on_all(zthread* threads, u64 count);

#endif