#ifndef DISJOINT_SET__H
#define DISJOINT_SET__H

#include "defines.h"

typedef struct disjoint_set disjoint_set;

disjoint_set* disjoint_set_create(u32 nodes);

void disjoint_set_destroy(disjoint_set* diset);

u32 disjoint_set_find_u_parent(disjoint_set* diset, u32 node);

void disjoint_set_union_by_rank(disjoint_set* diset, u32 u, u32 v);

void disjoint_set_union_by_size(disjoint_set* diset, u32 u, u32 v);

#endif