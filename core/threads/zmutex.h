#ifndef ZMUTEX__H
#define ZMUTEX__H

#include "defines.h"

typedef struct zmutex {
    void* internal_data;
} zmutex;

bool zmutex_create(zmutex* out_mutex);

void zmutex_destroy(zmutex* mutex);

bool zmutex_lock(zmutex* mutex);

bool zmutex_unlock(zmutex* mutex);

#endif