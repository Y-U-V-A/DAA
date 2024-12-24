#include "unordered_set.h"
#include "zmemory.h"
#include "logger.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                __                                      __                               __      //
//                                               /  |                                    /  |                             /  |     //
//  __    __  _______    ______    ______    ____$$ |  ______    ______    ______    ____$$ |         _______   ______   _$$ |_    //
// /  |  /  |/       \  /      \  /      \  /    $$ | /      \  /      \  /      \  /    $$ |        /       | /      \ / $$   |   //
// $$ |  $$ |$$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$ |       /$$$$$$$/ /$$$$$$  |$$$$$$/    //
// $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$/ $$ |  $$ |$$    $$ |$$ |  $$/ $$    $$ |$$ |  $$ |       $$      \ $$    $$ |  $$ | __  //
// $$ \__$$ |$$ |  $$ |$$ \__$$ |$$ |      $$ \__$$ |$$$$$$$$/ $$ |      $$$$$$$$/ $$ \__$$ |        $$$$$$  |$$$$$$$$/   $$ |/  | //
// $$    $$/ $$ |  $$ |$$    $$/ $$ |      $$    $$ |$$       |$$ |      $$       |$$    $$ |______ /     $$/ $$       |  $$  $$/  //
//  $$$$$$/  $$/   $$/  $$$$$$/  $$/        $$$$$$$/  $$$$$$$/ $$/        $$$$$$$/  $$$$$$$//      |$$$$$$$/   $$$$$$$/    $$$$/   //
//                                                                                          $$$$$$/                                //
//                                                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct unset_node {
    void* data;
    struct unset_node* next;
} unset_node;

typedef struct unordered_set {
    unset_node** array; // not darray
    u64 size;
    u64 cap;
    u64 data_stride;
    PFN_unordered_set_hash hash_func;
} unordered_set;

#define GENERATE_SET_HASH(un_set, data) un_set->hash_func(data, un_set->data_stride) % un_set->cap

unset_node* create_unset_node(const void* data, u64 data_stride);
void destroy_unset_node(unset_node* node, u64 data_stride);
u64 default_unordered_set_hash(const void* data, u64 data_stride);

unordered_set* _unordered_set_create(u64 size, u64 data_stride, PFN_unordered_set_hash hash_func) {

    if (hash_func == 0) {
        hash_func = default_unordered_set_hash;
    }

    unordered_set* temp = (unordered_set*)zmemory_allocate(sizeof(unordered_set), MEMORY_TAG_UNORDERED_SET);
    // sizeof(ptr)*UNORDERED_MAP_DEFAULT_SIZE;
    temp->array = (unset_node**)zmemory_allocate(sizeof(unset_node*) * size, MEMORY_TAG_UNORDERED_SET);
    temp->size = 0;
    temp->cap = size;
    temp->data_stride = data_stride;
    temp->hash_func = hash_func;
    return temp;
}

void unordered_set_destroy(unordered_set* un_set) {

    for (u32 i = 0; i < un_set->cap; ++i) {
        if (un_set->array[i]) {
            destroy_unset_node(un_set->array[i], un_set->data_stride);
            un_set->array[i] = 0;
        }
    }
    zmemory_free(un_set->array, sizeof(unset_node*) * un_set->cap, MEMORY_TAG_UNORDERED_SET);
    zmemory_free(un_set, sizeof(unordered_set), MEMORY_TAG_UNORDERED_SET);
}

unordered_set* _unordered_set_insert(unordered_set* un_set, const void* data) {

    if ((un_set->size / (f64)un_set->cap) >= UNORDERED_SET_LOAD_FACTOR) {
        un_set = unordered_set_resize(un_set);
    }

    u64 hash = GENERATE_SET_HASH(un_set, data);

    if (un_set->array[hash] == 0) {
        unset_node* node = create_unset_node(data, un_set->data_stride);
        un_set->array[hash] = node;
    } else {
        unset_node* curr = un_set->array[hash];
        unset_node* prev = 0;
        while (curr) {
            if (zmemory_compare(curr->data, data, un_set->data_stride) == 0) {
                return un_set;
            }
            prev = curr;
            curr = curr->next;
        }

        unset_node* node = create_unset_node(data, un_set->data_stride);
        prev->next = node;
    }

    un_set->size += 1;

    return un_set;
}

void unordered_set_remove(unordered_set* un_set, const void* data) {

    u64 hash = GENERATE_SET_HASH(un_set, data);

    unset_node* node = un_set->array[hash];

    if (zmemory_compare(node->data, data, un_set->data_stride) == 0) {
        un_set->array[hash] = node->next;
        node->next = 0;
        destroy_unset_node(node, un_set->data_stride);
        un_set->size -= 1;
        return;
    }

    unset_node* prev = 0;
    while (node) {
        if (zmemory_compare(node->data, data, un_set->data_stride) == 0) {
            prev->next = node->next;
            node->next = 0;
            destroy_unset_node(node, un_set->data_stride);
            un_set->size -= 1;
            return;
        }
        prev = node;
        node = node->next;
    }

    LOGW("unordered_set_data : invalid key");
}

unordered_set* unordered_set_resize(unordered_set* un_set) {

    unordered_set* temp = _unordered_set_create(UNORDERED_SET_RESIZE_FACTOR * un_set->cap, un_set->data_stride, un_set->hash_func);

    for (u32 i = 0; i < un_set->cap; ++i) {
        if (un_set->array[i]) {
            unset_node* node = un_set->array[i];

            while (node) {
                _unordered_set_insert(temp, node->data);
                node = node->next;
            }
        }
    }
    unordered_set_destroy(un_set);

    return temp;
}

u64 unordered_set_length(unordered_set* un_set) {
    return un_set->size;
}

u64 unordered_set_capacity(unordered_set* un_set) {
    return un_set->cap;
}

bool unordered_set_contains(unordered_set* un_set, const void* data) {

    u64 hash = GENERATE_SET_HASH(un_set, data);

    unset_node* node = un_set->array[hash];

    while (node) {
        if (zmemory_compare(node->data, data, un_set->data_stride) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
//  __                  __                                          //
// /  |                /  |                                         //
// $$ |____    ______  $$ |  ______    ______    ______    _______  //
// $$      \  /      \ $$ | /      \  /      \  /      \  /       | //
// $$$$$$$  |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$/  //
// $$ |  $$ |$$    $$ |$$ |$$ |  $$ |$$    $$ |$$ |  $$/ $$      \  //
// $$ |  $$ |$$$$$$$$/ $$ |$$ |__$$ |$$$$$$$$/ $$ |       $$$$$$  | //
// $$ |  $$ |$$       |$$ |$$    $$/ $$       |$$ |      /     $$/  //
// $$/   $$/  $$$$$$$/ $$/ $$$$$$$/   $$$$$$$/ $$/       $$$$$$$/   //
//                         $$ |                                     //
//                         $$ |                                     //
//                         $$/                                      //
//                                                                  //
//////////////////////////////////////////////////////////////////////

unset_node* create_unset_node(const void* data, u64 data_stride) {
    unset_node* temp = (unset_node*)zmemory_allocate(sizeof(unset_node), MEMORY_TAG_UNORDERED_SET);

    temp->data = zmemory_allocate(data_stride, MEMORY_TAG_UNORDERED_SET);
    zmemory_copy(temp->data, data, data_stride);

    temp->next = 0;

    return temp;
}

void destroy_unset_node(unset_node* node, u64 data_stride) {

    if (!node) {
        return;
    }

    destroy_unset_node(node->next, data_stride);

    zmemory_free(node->data, data_stride, MEMORY_TAG_UNORDERED_SET);
    zmemory_free(node, sizeof(unset_node), MEMORY_TAG_UNORDERED_SET);
    return;
}

u64 default_unordered_set_hash(const void* data, u64 data_stride) {

    u64 seed = 0xCBF29CE484222325ULL; // Example: 64-bit FNV offset basis

    const u8* val = (const u8*)data;

    for (u64 i = 0; i < data_stride; ++i) {
        seed ^= val[i];
        seed *= 1099511628211ULL; // 64-bit FNV prime
    }

    return seed;
}