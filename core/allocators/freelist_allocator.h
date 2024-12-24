#ifndef FREE_LIST_ALLOCATOR__H
#define FREE_LIST_ALLOCATOR__H

#include "defines.h"

typedef struct freelist_allocator freelist_allocator;

freelist_allocator* freelist_allocator_create(u64 size);

void freelist_allocator_destroy(freelist_allocator* allocator);

void* freelist_allocator_allocate(freelist_allocator* allocator, u64 size);

void freelist_allocator_free(freelist_allocator* allocator, void* block);

void freelist_allocator_reset(freelist_allocator* allocator);

u64 freelist_allocator_used_memory(freelist_allocator* allocator);

u64 freelist_allocator_unused_memory(freelist_allocator* allocator);

u64 freelist_allocator_header_size();

#endif