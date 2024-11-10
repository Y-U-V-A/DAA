#include "unordered_map.h"
#include "common.h"

typedef struct unmap_node {
    void* key;
    void* data;
    struct unmap_node* next;
} unmap_node;

struct unordered_map {
    unmap_node** array; // not darray
    u64 size;
    u64 cap;
    u64 key_stride;
    u64 data_stride;
    PFN_unordered_map_hash hash_func;
};

#define GENERATE_MAP_HASH(un_map, key) un_map->hash_func(key, un_map->key_stride) % un_map->cap

unmap_node* create_unmap_node(const void* key, const void* data, u64 key_stride, u64 data_stride);
void destroy_unmap_node(unmap_node* node, u64 key_stride, u64 data_stride);
u64 default_unordered_map_hash(const void* key, u64 key_stride);

unordered_map* _unordered_map_create(u64 size, u64 key_stride, u64 data_stride, PFN_unordered_map_hash hash_func) {

    if (hash_func == 0) {
        hash_func = default_unordered_map_hash;
    }

    unordered_map* temp = (unordered_map*)memory_allocate(sizeof(unordered_map), MEMORY_TAG_UNORDERED_MAP);
    // sizeof(ptr)*UNORDERED_MAP_DEFAULT_SIZE;
    temp->array = (unmap_node**)memory_allocate(sizeof(unmap_node*) * size, MEMORY_TAG_UNORDERED_MAP);
    temp->size = 0;
    temp->cap = size;
    temp->key_stride = key_stride;
    temp->data_stride = data_stride;
    temp->hash_func = hash_func;
    return temp;
}

void unordered_map_destroy(unordered_map* un_map) {

    for (u32 i = 0; i < un_map->cap; ++i) {
        if (un_map->array[i]) {
            destroy_unmap_node(un_map->array[i], un_map->key_stride, un_map->data_stride);
            un_map->array[i] = 0;
        }
    }
    memory_free(un_map->array, sizeof(unmap_node*) * un_map->cap, MEMORY_TAG_UNORDERED_MAP);
    memory_free(un_map, sizeof(unordered_map), MEMORY_TAG_UNORDERED_MAP);
}

unordered_map* _unordered_map_insert(unordered_map* un_map, const void* key, const void* data) {

    if ((un_map->size / (f64)un_map->cap) >= UNORDERED_MAP_LOAD_FACTOR) {
        un_map = unordered_map_resize(un_map);
    }

    u64 hash = GENERATE_MAP_HASH(un_map, key);

    if (un_map->array[hash] == 0) {
        unmap_node* node = create_unmap_node(key, data, un_map->key_stride, un_map->data_stride);
        un_map->array[hash] = node;
    } else {
        unmap_node* curr = un_map->array[hash];
        unmap_node* prev = 0;
        while (curr) {
            if (memory_compare(curr->key, key, un_map->key_stride) == 0) {
                memory_copy(curr->data, data, un_map->data_stride);
                return un_map;
            }
            prev = curr;
            curr = curr->next;
        }

        unmap_node* node = create_unmap_node(key, data, un_map->key_stride, un_map->data_stride);
        prev->next = node;
    }

    un_map->size += 1;
    return un_map;
}

void* unordered_map_data(unordered_map* un_map, const void* key) {

    u64 hash = GENERATE_MAP_HASH(un_map, key);

    unmap_node* node = un_map->array[hash];

    while (node) {
        if (memory_compare(node->key, key, un_map->key_stride) == 0) {
            return node->data;
        }
        node = node->next;
    }

    LOGW("unordered_map_data : invalid key");

    return 0;
}

void unordered_map_remove(unordered_map* un_map, const void* key) {

    u64 hash = GENERATE_MAP_HASH(un_map, key);

    unmap_node* node = un_map->array[hash];

    if (memory_compare(node->key, key, un_map->key_stride) == 0) {
        un_map->array[hash] = node->next;
        node->next = 0;
        destroy_unmap_node(node, un_map->key_stride, un_map->data_stride);
        un_map->size -= 1;
        return;
    }

    unmap_node* prev = 0;
    while (node) {
        if (memory_compare(node->key, key, un_map->key_stride) == 0) {
            prev->next = node->next;
            node->next = 0;
            destroy_unmap_node(node, un_map->key_stride, un_map->data_stride);
            un_map->size -= 1;
            return;
        }
        prev = node;
        node = node->next;
    }

    LOGW("unordered_map_data : invalid key");
}

unordered_map* unordered_map_resize(unordered_map* un_map) {

    unordered_map* temp = _unordered_map_create(UNORDERED_MAP_RESIZE_FACTOR * un_map->cap,
                                                un_map->key_stride, un_map->data_stride, un_map->hash_func);

    for (u32 i = 0; i < un_map->cap; ++i) {
        if (un_map->array[i]) {
            unmap_node* node = un_map->array[i];

            while (node) {
                _unordered_map_insert(temp, node->key, node->data);
                node = node->next;
            }
        }
    }

    unordered_map_destroy(un_map);
    return temp;
}

u64 unordered_map_size(unordered_map* un_map) {
    return un_map->size;
}

u64 unordered_map_capacity(unordered_map* un_map) {
    return un_map->cap;
}

bool unordered_map_contains(unordered_map* un_map, const void* key) {
    u64 hash = GENERATE_MAP_HASH(un_map, key);

    unmap_node* node = un_map->array[hash];

    while (node) {
        if (memory_compare(node->key, key, un_map->key_stride) == 0) {

            return true;
        }
        node = node->next;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

unmap_node* create_unmap_node(const void* key, const void* data, u64 key_stride, u64 data_stride) {
    unmap_node* temp = (unmap_node*)memory_allocate(sizeof(unmap_node), MEMORY_TAG_UNORDERED_MAP);

    temp->key = memory_allocate(key_stride, MEMORY_TAG_UNORDERED_MAP);
    memory_copy(temp->key, key, key_stride);

    temp->data = memory_allocate(data_stride, MEMORY_TAG_UNORDERED_MAP);
    memory_copy(temp->data, data, data_stride);

    temp->next = 0;

    return temp;
}

void destroy_unmap_node(unmap_node* node, u64 key_stride, u64 data_stride) {

    if (!node) {
        return;
    }

    destroy_unmap_node(node->next, key_stride, data_stride);

    memory_free(node->key, key_stride, MEMORY_TAG_UNORDERED_MAP);
    memory_free(node->data, data_stride, MEMORY_TAG_UNORDERED_MAP);
    memory_free(node, sizeof(unmap_node), MEMORY_TAG_UNORDERED_MAP);
    return;
}

u64 default_unordered_map_hash(const void* key, u64 key_stride) {

    u64 seed = 0xCBF29CE484222325ULL; // Example: 64-bit FNV offset basis

    const u8* data = (const u8*)key;

    for (u64 i = 0; i < key_stride; ++i) {
        seed ^= data[i];
        seed *= 1099511628211ULL; // 64-bit FNV prime
    }

    return seed;
}