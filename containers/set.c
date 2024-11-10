#include "set.h"
#include "common.h"

set_node* create_set_node(const void* data, u64 data_stride);
void destroy_set_node(set_node* node, u64 data_stride);

// node will become left child of node's right child ,
void left_rotate_set_node(set* st, set_node* node);
// node will become right child of node's left child ,
void right_rotate_set_node(set* st, set_node* node);

void insert_fix_up_set_node(set* st, set_node* node);

void remove_fix_up_set_node(set* st, set_node* node);

struct set {
    set_node* root;
    u64 size;
    u64 data_stride;
    PFN_set_cmp cmp_func;
};

set* _set_create(u64 data_stride, PFN_set_cmp cmp_func) {

    set* temp = memory_allocate(sizeof(set), MEMORY_TAG_SET);
    temp->root = 0;
    temp->size = 0;
    temp->data_stride = data_stride;
    temp->cmp_func = cmp_func;

    return temp;
}

void set_destroy(set* st) {
    if (st->root) {
        destroy_set_node(st->root, st->data_stride);
    }
    memory_free(st, sizeof(set), MEMORY_TAG_SET);
}

void set_insert(set* st, const void* data) {
    set_node* node = create_set_node(data, st->data_stride);

    if (st->root == 0) {
        st->root = node;
        st->root->is_red = 0;
    } else {
        set_node* temp = st->root;

        while (temp) {

            if (memory_compare(temp->data, data, st->data_stride) == 0) {
                memory_copy(temp->data, data, st->data_stride);
                destroy_set_node(node, st->data_stride);
                return;
            }

            if (st->cmp_func(temp->data, data)) {
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

        insert_fix_up_set_node(st, node);
    }
    st->size += 1;
}

void set_remove(set* st, const void* data) {

    if (st->root == 0) {
        LOGW("set_remove : set has zero nodes ");
        return;
    }

    set_node* node = st->root;

    void* k = memory_allocate(st->data_stride, MEMORY_TAG_SET);
    memory_copy(k, data, st->data_stride);

    while (node) {

        if (memory_compare(node->data, k, st->data_stride) == 0) {

            if (node->left == 0 && node->right == 0) {

                if (node->is_red == 0) {
                    remove_fix_up_set_node(st, node);
                }

                if (node->parent) {

                    if (node->parent->left == node) {
                        node->parent->left = 0;
                    } else {
                        node->parent->right = 0;
                    }
                } else {
                    st->root = 0;
                }

                destroy_set_node(node, st->data_stride);

                st->size -= 1;

                memory_free(k, st->data_stride, MEMORY_TAG_SET);

                return;

            } else if (node->right == 0) {

                set_node* prev = node->left;
                while (prev->right) {
                    prev = prev->right;
                }

                memory_copy(node->data, prev->data, st->data_stride);

                memory_copy(k, prev->data, st->data_stride);

                node = node->left;

            } else {

                set_node* next = node->right;
                while (next->left) {
                    next = next->left;
                }

                memory_copy(node->data, next->data, st->data_stride);

                memory_copy(k, next->data, st->data_stride);

                node = node->right;
            }

        } else {

            if (st->cmp_func(node->data, k)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
    }

    LOGW("set_remove : invalid key");

    memory_free(k, st->data_stride, MEMORY_TAG_SET);

    return;
}

u64 set_size(set* st) {
    return st->size;
}

set_node* set_begin(set* st) {
    set_node* temp = st->root;
    while (temp->left) {
        temp = temp->left;
    }
    return temp;
}

set_node* set_next(set* st, set_node* node) {
    if (node->right) {
        set_node* temp = node->right;
        while (temp->left) {
            temp = temp->left;
        }
        return temp;
    }

    set_node* temp = st->root;
    set_node* next;

    while (temp) {

        if (memory_compare(temp->data, node->data, st->data_stride) == 0) {
            break;
        }

        if (st->cmp_func(temp->data, node->data)) {
            next = temp;
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    return next;
}

set_node* set_end(set* st) {
    set_node* temp = st->root;
    while (temp->right) {
        temp = temp->right;
    }
    return temp;
}

bool set_contains(set* st, const void* data) {

    set_node* temp = st->root;
    while (temp) {

        if (memory_compare(temp->data, data, st->data_stride) == 0) {
            return true;
        }

        if (st->cmp_func(temp->data, data)) {

            temp = temp->left;
        } else {

            temp = temp->right;
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

set_node* create_set_node(const void* data, u64 data_stride) {
    set_node* temp = memory_allocate(sizeof(set_node), MEMORY_TAG_SET);

    temp->data = memory_allocate(data_stride, MEMORY_TAG_SET);
    memory_copy(temp->data, data, data_stride);

    temp->left = 0;
    temp->right = 0;
    temp->parent = 0;
    temp->is_red = 1;

    return temp;
}

void destroy_set_node(set_node* node, u64 data_stride) {
    if (!node) {
        return;
    }

    destroy_set_node(node->left, data_stride);
    destroy_set_node(node->right, data_stride);

    memory_free(node->data, data_stride, MEMORY_TAG_SET);
    memory_free(node, sizeof(set_node), MEMORY_TAG_SET);
}

// node will become left child of node's right child ,
void left_rotate_set_node(set* st, set_node* node) {

    if (!node || !node->right)
        return; // Safety check

    set_node* parent = node->parent;
    set_node* child = node->right;

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
        st->root = child;
    }
}

// node will become right child of node's left child ,
void right_rotate_set_node(set* st, set_node* node) {

    if (!node || !node->left)
        return; // Safety check

    set_node* parent = node->parent;
    set_node* child = node->left;

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
        st->root = child;
    }
}

void insert_fix_up_set_node(set* st, set_node* node) {

    while (node->parent && node->parent->is_red) {

        if (node->parent->parent) {

            set_node* uncle;
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
                        left_rotate_set_node(st, node);
                    }
                    right_rotate_set_node(st, node->parent->parent);
                    node->parent->is_red = 0;
                    node->parent->right->is_red = 1;

                } else {
                    if (node->parent->left == node) {
                        node = node->parent;
                        right_rotate_set_node(st, node);
                    }
                    left_rotate_set_node(st, node->parent->parent);
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

void remove_fix_up_set_node(set* st, set_node* node) {

    if (node->parent == 0) {
        return;
    }

    set_node* sibling;

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
                right_rotate_set_node(st, node->parent);
                sibling = node->parent->left;
            } else {
                left_rotate_set_node(st, node->parent);
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
            set_node *far, *near;

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
                    right_rotate_set_node(st, sibling);
                } else {
                    left_rotate_set_node(st, sibling);
                }

                if (node->parent->left == node) {
                    sibling = node->parent->right;
                } else {
                    sibling = node->parent->left;
                }
            }

            if (node == node->parent->right) {
                right_rotate_set_node(st, sibling->parent);
                if (node->parent->is_red == 0) {
                    sibling->left->is_red = 0;
                }

            } else {
                left_rotate_set_node(st, sibling->parent);
                if (node->parent->is_red == 0) {
                    sibling->right->is_red = 0;
                }
            }

            break;
        }
    }
}