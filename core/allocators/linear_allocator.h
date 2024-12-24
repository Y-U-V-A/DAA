#ifndef LINEAR_ALLOCATOR__H
#define LINEAR_ALLOCATOR__H

#include "defines.h"

typedef enum linear_allocator_memory_alignment {
    ALIGNMENT_BYTE_8 = 8,
    ALIGNMENT_BYTE_16 = 16,
    ALIGNMENT_BYTE_32 = 32,
    ALIGNMENT_BYTE_64 = 64,
} linear_allocator_memory_alignment;

#define linear_allocator_allocate(allocator, size) linear_allocator_allocate_aligned(allocator, size, ALIGNMENT_BYTE_8)
#define linear_allocator_allocate_aligned_8(allocator, size) linear_allocator_allocate_aligned(allocator, size, ALIGNMENT_BYTE_8)
#define linear_allocator_allocate_aligned_16(allocator, size) linear_allocator_allocate_aligned(allocator, size, ALIGNMENT_BYTE_16)
#define linear_allocator_allocate_aligned_32(allocator, size) linear_allocator_allocate_aligned(allocator, size, ALIGNMENT_BYTE_32)
#define linear_allocator_allocate_aligned_64(allocator, size) linear_allocator_allocate_aligned(allocator, size, ALIGNMENT_BYTE_64)

typedef struct linear_allocator linear_allocator;

linear_allocator* linear_allocator_create(u64 size);

void linear_allocator_destroy(linear_allocator* allocator);

void* linear_allocator_allocate_aligned(linear_allocator* allocator, u64 size, linear_allocator_memory_alignment memory_alignment);

void linear_allocator_reset(linear_allocator* allocator);

u64 linear_allocator_used_memory(linear_allocator* allocator);

u64 linear_allocator_unused_memory(linear_allocator* allocator);

#endif