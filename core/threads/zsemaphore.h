#ifndef ZSEMAPHORE__H
#define ZSEMAPHORE__H

#include "defines.h"

typedef struct zsemaphore {
    void* internal_data;
} zsemaphore;

bool zsemaphore_create(u32 max_count, zsemaphore* out_semaphore);

void zsemaphore_destroy(zsemaphore* semaphore);

bool zsemaphore_signal(zsemaphore* semaphore);

bool zsemaphore_wait(zsemaphore* semaphore);

#endif