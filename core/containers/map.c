#include "map.h"
#include "common.h"
#include "logger.h"

map_node* create_map_node(const void* key, const void* data, u64 key_stride, u64 data_stride);
void destroy_map_node(map_node* node, u64 key_stride, u64 data_stride);

// node will become left child of node's right child ,
void left_rotate_map_node(map* mp, map_node* node);
// node will become right child of node's left child ,
void right_rotate_map_node(map* mp, map_node* node);

void insert_fix_up_map_node(map* mp, map_node* node);

void remove_fix_up_map_node(map* mp, map_node* node);

struct map {
    map_node* root;
    u64 size;
    u64 key_stride;
    u64 data_stride;
    PFN_map_cmp cmp_func;
};

map* _map_create(u64 key_stride, u64 data_stride, PFN_map_cmp cmp_func) {

    map* temp = memory_allocate(sizeof(map), MEMORY_TAG_MAP);
    temp->root = 0;
    temp->size = 0;
    temp->key_stride = key_stride;
    temp->data_stride = data_stride;
    temp->cmp_func = cmp_func;

    return temp;
}

void map_destroy(map* mp) {
    if (mp->root) {
        destroy_map_node(mp->root, mp->key_stride, mp->data_stride);
    }
    memory_free(mp, sizeof(map), MEMORY_TAG_MAP);
}

void map_insert(map* mp, const void* key, const void* data) {
    map_node* node = create_map_node(key, data, mp->key_stride, mp->data_stride);

    if (mp->root == 0) {
        mp->root = node;
        mp->root->is_red = 0;
    } else {
        map_node* temp = mp->root;

        while (temp) {

            if (memory_compare(temp->key, key, mp->key_stride) == 0) {
                memory_copy(temp->data, data, mp->data_stride);
                destroy_map_node(node, mp->key_stride, mp->data_stride);
                return;
            }

            if (mp->cmp_func(temp->key, key)) {
                if (temp->left == 0) {
                    temp->left = node;
                    break;
                }
                temp = temp->left;
            } else {
                if (temp->right == 0) {
                    temp->right = node;
                    break;
                }
                temp = temp->right;
            }
        }
        node->parent = temp;

        insert_fix_up_map_node(mp, node);
    }
    mp->size += 1;
}

void map_remove(map* mp, const void* key) {

    if (mp->root == 0) {
        LOGW("map_remove : map has zero nodes ");
        return;
    }

    map_node* node = mp->root;

    void* k = memory_allocate(mp->key_stride, MEMORY_TAG_MAP);
    memory_copy(k, key, mp->key_stride);

    while (node) {

        if (memory_compare(node->key, k, mp->key_stride) == 0) {

            if (node->left == 0 && node->right == 0) {

                if (node->is_red == 0) {
                    remove_fix_up_map_node(mp, node);
                }

                if (node->parent) {

                    if (node->parent->left == node) {
                        node->parent->left = 0;
                    } else {
                        node->parent->right = 0;
                    }
                } else {
                    mp->root = 0;
                }

                destroy_map_node(node, mp->key_stride, mp->data_stride);

                mp->size -= 1;

                memory_free(k, mp->key_stride, MEMORY_TAG_MAP);

                return;

            } else if (node->right == 0) {

                map_node* prev = node->left;
                while (prev->right) {
                    prev = prev->right;
                }

                memory_copy(node->key, prev->key, mp->key_stride);
                memory_copy(node->data, prev->data, mp->data_stride);

                memory_copy(k, prev->key, mp->key_stride);

                node = node->left;

            } else {

                map_node* next = node->right;
                while (next->left) {
                    next = next->left;
                }

                memory_copy(node->key, next->key, mp->key_stride);
                memory_copy(node->data, next->data, mp->data_stride);

                memory_copy(k, next->key, mp->key_stride);
                node = node->right;
            }

        } else {

            if (mp->cmp_func(node->key, k)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
    }

    LOGW("map_remove : invalid key");

    memory_free(k, mp->key_stride, MEMORY_TAG_MAP);

    return;
}

u64 map_size(map* mp) {
    return mp->size;
}

void* map_data(map* mp, const void* key) {

    if (mp->root == 0) {
        LOGW("map_remove : map has zero nodes ");
        return 0;
    }

    map_node* temp = mp->root;

    while (temp) {

        if (memory_compare(temp->key, key, mp->key_stride) == 0) {
            return temp->data;
        }

        if (mp->cmp_func(temp->key, key)) {

            temp = temp->left;
        } else {

            temp = temp->right;
        }
    }

    LOGW("map_data : invalid key");

    return 0;
}

map_node* map_begin(map* mp) {
    map_node* temp = mp->root;
    while (temp->left) {
        temp = temp->left;
    }
    return temp;
}

map_node* map_next(map* mp, map_node* node) {
    if (node->right) {
        map_node* temp = node->right;
        while (temp->left) {
            temp = temp->left;
        }
        return temp;
    }

    map_node* temp = mp->root;
    map_node* next;

    while (temp) {

        if (memory_compare(temp->key, node->key, mp->key_stride) == 0) {
            break;
        }

        if (mp->cmp_func(temp->key, node->key)) {
            next = temp;
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    return next;
}

map_node* map_end(map* mp) {
    map_node* temp = mp->root;
    while (temp->right) {
        temp = temp->right;
    }
    return temp;
}

bool map_contains(map* mp, const void* key) {

    map_node* temp = mp->root;
    while (temp) {

        if (memory_compare(temp->key, key, mp->key_stride) == 0) {
            return true;
        }

        if (mp->cmp_func(temp->key, key)) {

            temp = temp->left;
        } else {

            temp = temp->right;
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

map_node* create_map_node(const void* key, const void* data, u64 key_stride, u64 data_stride) {
    map_node* temp = memory_allocate(sizeof(map_node), MEMORY_TAG_MAP);

    temp->key = memory_allocate(key_stride, MEMORY_TAG_MAP);
    memory_copy(temp->key, key, key_stride);

    temp->data = memory_allocate(data_stride, MEMORY_TAG_MAP);
    memory_copy(temp->data, data, data_stride);

    temp->left = 0;
    temp->right = 0;
    temp->parent = 0;
    temp->is_red = 1;

    return temp;
}

void destroy_map_node(map_node* node, u64 key_stride, u64 data_stride) {
    if (node == 0) {
        return;
    }

    destroy_map_node(node->left, key_stride, data_stride);
    destroy_map_node(node->right, key_stride, data_stride);

    memory_free(node->key, key_stride, MEMORY_TAG_MAP);
    memory_free(node->data, data_stride, MEMORY_TAG_MAP);
    memory_free(node, sizeof(map_node), MEMORY_TAG_MAP);
}

// node will become left child of node's right child ,
void left_rotate_map_node(map* mp, map_node* node) {

    if (!node || !node->right)
        return; // Safety check

    map_node* parent = node->parent;
    map_node* child = node->right;

    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }
    child->parent = parent;

    node->right = child->left;
    if (node->right) {
        node->right->parent = node;
    }

    child->left = node;
    node->parent = child;

    if (child->parent == 0) {
        mp->root = child;
    }
}

// node will become right child of node's left child ,
void right_rotate_map_node(map* mp, map_node* node) {

    if (!node || !node->left)
        return; // Safety check

    map_node* parent = node->parent;
    map_node* child = node->left;

    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }
    child->parent = parent;

    node->left = child->right;
    if (node->left) {
        node->left->parent = node;
    }

    child->right = node;
    node->parent = child;

    if (child->parent == 0) {
        mp->root = child;
    }
}

void insert_fix_up_map_node(map* mp, map_node* node) {

    while (node->parent && node->parent->is_red) {

        if (node->parent->parent) {

            map_node* uncle;
            if (node->parent->parent->left == node->parent) {
                uncle = node->parent->parent->right;

            } else {
                uncle = node->parent->parent->left;
            }

            if (uncle && uncle->is_red) {
                uncle->is_red = 0;
                node->parent->is_red = 0;
                node->parent->parent->is_red = 1;

                node = node->parent->parent;
            } else {

                if (node->parent->parent->left == node->parent) {

                    if (node->parent->right == node) {
                        node = node->parent;
                        left_rotate_map_node(mp, node);
                    }
                    right_rotate_map_node(mp, node->parent->parent);
                    node->parent->is_red = 0;
                    node->parent->right->is_red = 1;

                } else {
                    if (node->parent->left == node) {
                        node = node->parent;
                        right_rotate_map_node(mp, node);
                    }
                    left_rotate_map_node(mp, node->parent->parent);
                    node->parent->is_red = 0;
                    node->parent->left->is_red = 1;
                }
                break;
            }
        } else {
            node->parent->is_red = 0;
        }
    }
    if (node->parent == 0) {
        node->is_red = 0;
    }
}

// TODO: fix this

void remove_fix_up_map_node(map* mp, map_node* node) {

    if (node->parent == 0) {
        return;
    }

    map_node* sibling;

    if (node->parent->left == node) {
        sibling = node->parent->right;
    } else {
        sibling = node->parent->left;
    }

    while (1) {

        if (sibling && sibling->is_red) {

            sibling->is_red = 0;
            sibling->parent->is_red = 1;

            if (node->parent->left == sibling) {
                right_rotate_map_node(mp, node->parent);
                sibling = node->parent->left;
            } else {
                left_rotate_map_node(mp, node->parent);
                sibling = node->parent->right;
            }
            continue;
        }

        if (sibling == 0 || ((sibling->left == 0 || sibling->left->is_red == 0) && (sibling->right == 0 || sibling->right->is_red == 0))) {

            if (node->parent->is_red) {
                node->parent->is_red = 0;

                if (sibling) {
                    sibling->is_red = 1;
                }

                break;
            } else {

                if (sibling) {
                    sibling->is_red = 1;
                }

                node = node->parent;

                if (node->parent == 0) {
                    node->is_red = 0;
                    break;
                }

                if (node->parent->left == node) {
                    sibling = node->parent->right;
                } else {
                    sibling = node->parent->left;
                }
            }
        } else {
            map_node *far, *near;

            if (node->parent->left == node) {
                far = sibling->right;
                near = sibling->left;
            } else {
                far = sibling->left;
                near = sibling->right;
            }

            if (near && near->is_red && (far == 0 || far->is_red == 0)) {
                sibling->is_red = 1;
                near->is_red = 0;

                if (near == sibling->left) {
                    right_rotate_map_node(mp, sibling);
                } else {
                    left_rotate_map_node(mp, sibling);
                }

                if (node->parent->left == node) {
                    sibling = node->parent->right;
                } else {
                    sibling = node->parent->left;
                }
            }

            if (node == node->parent->right) {
                right_rotate_map_node(mp, sibling->parent);
                if (node->parent->is_red == 0) {
                    sibling->left->is_red = 0;
                }

            } else {
                left_rotate_map_node(mp, sibling->parent);
                if (node->parent->is_red == 0) {
                    sibling->right->is_red = 0;
                }
            }

            break;
        }
    }
}