#include "disjoint_set.h"
#include "zmemory.h"
#include "logger.h"

typedef struct disjoint_set {
    u32* parent;
    u32* rank;
    u32* size;
    u32 nodes;
} disjoint_set;

disjoint_set* disjoint_set_create(u32 nodes) {
    if (nodes == 0) {
        LOGE("disjoint_set_create : invalid params");
        return 0;
    }
    disjoint_set* diset = zmemory_allocate(sizeof(disjoint_set), MEMORY_DISJOINT_SET);
    diset->parent = zmemory_allocate(sizeof(u32) * nodes, MEMORY_DISJOINT_SET);
    diset->rank = zmemory_allocate(sizeof(u32) * nodes, MEMORY_DISJOINT_SET);
    diset->size = zmemory_allocate(sizeof(u32) * nodes, MEMORY_DISJOINT_SET);
    diset->nodes = nodes;

    for (u32 i = 0; i < nodes; ++i) {
        diset->parent[i] = i;
        diset->rank[i] = 0;
        diset->size[i] = 1;
    }
    return diset;
}

void disjoint_set_destroy(disjoint_set* diset) {
    if (!diset) {
        LOGE("disjoint_set_destroy : invalid params");
        return;
    }

    zmemory_free(diset->parent, sizeof(u32) * diset->nodes, MEMORY_DISJOINT_SET);
    zmemory_free(diset->rank, sizeof(u32) * diset->nodes, MEMORY_DISJOINT_SET);
    zmemory_free(diset->size, sizeof(u32) * diset->nodes, MEMORY_DISJOINT_SET);
    zmemory_free(diset, sizeof(disjoint_set), MEMORY_DISJOINT_SET);
}

u32 disjoint_set_find_u_parent(disjoint_set* diset, u32 node) {
    if (diset->parent[node] == node) {
        return node;
    }
    return diset->parent[node] = disjoint_set_find_u_parent(diset, diset->parent[node]);
}

void disjoint_set_union_by_rank(disjoint_set* diset, u32 u, u32 v) {
    u32 up = disjoint_set_find_u_parent(diset, u);
    u32 vp = disjoint_set_find_u_parent(diset, v);
    if (up == vp)
        return;
    if (diset->rank[up] > diset->rank[vp]) {
        diset->parent[vp] = up;
    } else if (diset->rank[up] < diset->rank[vp]) {
        diset->parent[up] = vp;
    } else {
        diset->parent[vp] = up;
        diset->rank[up] += 1;
    }
}

void disjoint_set_union_by_size(disjoint_set* diset, u32 u, u32 v) {
    u32 up = disjoint_set_find_u_parent(diset, u);
    u32 vp = disjoint_set_find_u_parent(diset, v);
    if (up == vp)
        return;
    if (diset->size[up] > diset->size[vp]) {
        diset->parent[vp] = up;
        diset->size[up] += diset->size[vp];
    } else {
        diset->parent[up] = vp;
        diset->size[vp] += diset->size[up];
    }
}