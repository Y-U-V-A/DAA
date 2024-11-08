#ifndef MAP_H
#define MAP_H

#include "defines.h"

typedef struct map map;

typedef b8 (*PFN_map_cmp)(const void* root_key, const void* node_key);

#define map_create(key_type, data_type, PFN_map_cmp) _map_create(sizeof(key_type), sizeof(data_type), PFN_map_cmp)

#endif