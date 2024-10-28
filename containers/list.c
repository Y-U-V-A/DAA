#include "list.h"
#include "common.h"

// using singly linked list
typedef struct list_node {
    void* data;
    struct list_node* next;
} list_node;

struct list {
    list_node* node;
    u64 size;
    u64 stride;
};

list_node* create_list_node(void* data, u64 stride);
void destroy_list_node(list_node* node, u64 stride);
list_node* reverse_list_nodes(list_node* node);
list_node* sort_list_nodes(list_node* head, PFN_list_cmp cmp_func);

list* _list_create(u64 stride) {
    list* temp = (list*)memory_allocate(sizeof(list), MEMORY_TAG_LIST);
    temp->size = 0;
    temp->node = 0;
    temp->stride = stride;

    return temp;
}

void list_destroy(list* lst) {

    if (lst->node)
        destroy_list_node(lst->node, lst->stride);

    memory_free(lst, sizeof(list), MEMORY_TAG_LIST);
}

void list_push_back(list* lst, void* data) {
    list_node* node = create_list_node(data, lst->stride);

    if (!lst->node) {
        lst->node = node;
    } else {
        list_node* temp = lst->node;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = node;
    }
    lst->size += 1;
}

void list_pop_back(list* lst) {

    if (!lst->node) {
        LOGW("list_pop_back : list has zero nodes");
        return;
    }

    if (!lst->node->next) {
        list_node* temp = lst->node;
        lst->node = 0;
        destroy_list_node(temp, lst->stride);

    } else {
        list_node* temp = lst->node;
        while (temp->next->next) {
            temp = temp->next;
        }
        destroy_list_node(temp->next, lst->stride);
        temp->next = 0;
    }
    lst->size -= 1;
}

void list_push_front(list* lst, void* data) {
    list_node* node = create_list_node(data, lst->stride);

    if (!lst->node) {
        lst->node = node;
    } else {
        list_node* temp = lst->node;
        node->next = temp;
        lst->node = node;
    }
    lst->size += 1;
}

void list_pop_front(list* lst) {

    if (!lst->node) {
        LOGW("list_pop_front : list has zero nodes");
        return;
    }

    if (!lst->node->next) {
        list_node* temp = lst->node;
        lst->node = 0;
        destroy_list_node(temp, lst->stride);
    } else {
        list_node* temp = lst->node;
        lst->node = temp->next;

        temp->next = 0;

        destroy_list_node(temp, lst->stride);
    }
    lst->size -= 1;
}

void list_insert(list* lst, u64 index, void* data) {

    if (index > lst->size) {
        LOGW("list_insert : index out of range");
        return;
    }

    if (index == 0) {
        list_push_front(lst, data);
        return;
    }

    if (index == lst->size) {
        list_push_back(lst, data);
        return;
    }

    list_node* node = create_list_node(data, lst->stride);

    list_node* temp = lst->node;

    while (--index) {
        temp = temp->next;
    }

    node->next = temp->next;
    temp->next = node;

    lst->size += 1;
}

void list_remove(list* lst, u64 index) {

    if (index > lst->size - 1) {
        LOGW("list_remove : index out of range");
        return;
    }

    if (index == 0) {
        list_pop_front(lst);
        return;
    }

    if (index == lst->size - 1) {
        list_pop_back(lst);
        return;
    }

    list_node* temp = lst->node;
    while (--index) {
        temp = temp->next;
    }

    list_node* x = temp->next;
    temp->next = x->next;
    x->next = 0;

    destroy_list_node(x, lst->stride);

    lst->size -= 1;
}

void list_clear(list* lst) {
    if (lst->node) {
        destroy_list_node(lst->node, lst->stride);
        lst->size = 0;
    }
}

u64 list_size(list* lst) {
    return lst->size;
}

void* list_data(list* lst, u64 index) {

    if (index > lst->size - 1) {
        LOGW("list_data : invaild index");
        return 0;
    }

    list_node* temp = lst->node;
    while (index--) {
        temp = temp->next;
    }

    return temp->data;
}

// uses merge sort;
void list_sort(list* lst, PFN_list_cmp cmp_func) {

    if (!lst->node) {
        LOGW("list_sort : list has zero nodes");
        return;
    }

    lst->node = sort_list_nodes(lst->node, cmp_func);
}

void list_merge(list* lst_final, list* lst, PFN_list_cmp cmp_func) {

    list_node* head1 = lst_final->node;
    list_node* head2 = lst->node;

    if (head1 == 0) {
        lst_final->node = head2;
        return;
    }
    if (head2 == 0) {
        return;
    }

    list_node* dummy = 0;

    if (cmp_func(head1->data, head2->data)) {
        dummy = head1;
        head1 = head1->next;
    } else {
        dummy = head2;
        head2 = head2->next;
    }

    list_node* curr = dummy;

    while (head1 && head2) {
        if (cmp_func(head1->data, head2->data)) {
            curr->next = head1;
            head1 = head1->next;
        } else {
            curr->next = head2;
            head2 = head2->next;
        }
        curr = curr->next;
    }

    curr->next = (head1 ? head1 : head2);

    lst_final->node = dummy;
    lst_final->size += lst->size;
    lst->node = 0;
    lst->size = 0;

    return;
}

void list_reverse(list* lst) {

    list_node* first = lst->node;
    while (first->next) {
        first = first->next;
    }

    list_node* last = reverse_list_nodes(lst->node);
    last->next = 0;

    lst->node = first;
}

void list_unique(list* sorted_lst, PFN_list_cmp cmp_func) {

    list_node* node = sorted_lst->node;

    while (node->next) {
        if (cmp_func(node->data, node->next->data)) {

            list_node* x = node->next;
            node->next = x->next;
            x->next = 0;
            destroy_list_node(x, sorted_lst->stride);
            sorted_lst->size -= 1;

            continue;
        }
        node = node->next;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

list_node* create_list_node(void* data, u64 stride) {
    list_node* temp = (list_node*)memory_allocate(sizeof(list_node), MEMORY_TAG_LIST);
    temp->data = memory_allocate(stride, MEMORY_TAG_LIST);
    memory_copy(temp->data, data, stride);
    temp->next = 0;

    return temp;
}

void destroy_list_node(list_node* node, u64 stride) {
    if (!node->next) {
        memory_free(node->data, stride, MEMORY_TAG_LIST);
        memory_free(node, sizeof(list_node), MEMORY_TAG_LIST);
        return;
    }

    destroy_list_node(node->next, stride);

    memory_free(node->data, stride, MEMORY_TAG_LIST);
    memory_free(node, sizeof(list_node), MEMORY_TAG_LIST);
    return;
}

list_node* reverse_list_nodes(list_node* node) {
    if (node->next == 0) {
        return node;
    }

    list_node* temp = reverse_list_nodes(node->next);

    temp->next = node;

    return node;
}

list_node* sort_list_nodes(list_node* head, PFN_list_cmp cmp_func) {
    if (head == 0 || head->next == 0) {
        return head;
    }

    list_node* fast = head;
    list_node* slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        if (fast) {
            slow = slow->next;
        }
    }

    list_node* mid = slow->next;
    slow->next = 0;

    list_node* head1 = sort_list_nodes(head, cmp_func);
    list_node* head2 = sort_list_nodes(mid, cmp_func);

    if (head1 == 0)
        return head2;
    if (head2 == 0)
        return head1;

    list_node* dummy = 0;

    if (cmp_func(head1->data, head2->data)) {
        dummy = head1;
        head1 = head1->next;
    } else {
        dummy = head2;
        head2 = head2->next;
    }

    list_node* curr = dummy;

    while (head1 && head2) {
        if (cmp_func(head1->data, head2->data)) {
            curr->next = head1;
            head1 = head1->next;
        } else {
            curr->next = head2;
            head2 = head2->next;
        }
        curr = curr->next;
    }

    curr->next = (head1 ? head1 : head2);

    return dummy;
}
