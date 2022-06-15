#include <stdlib.h>
#include <stdio.h>

typedef struct list_node_t {
    void               *value;
    struct list_node_t *next;
    struct list_node_t *prev;
} list_node_t;

typedef struct list_t {
    size_t       length;
    list_node_t *first;
    list_node_t *last;
} list_t;

list_t *list_create() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->length = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

static void list_add(list_t *list, void *arg) {
    if (list->first) {
        list->last->next = (list_node_t *)malloc(sizeof(list_node_t));
        list->last->next->value = arg;
        list->last->next->next = NULL;
        list->last->next->prev = list->last;
        list->last = list->last->next;
    } else {
        list->first = (list_node_t *)malloc(sizeof(list_node_t));
        list->first->value = arg;
        list->first->next = NULL;
        list->first->prev = NULL;
        list->last = list->first;
    }
    ++list->length;
}

static void list_pop_back(list_t *list) {
    list_node_t *old = list->last;
    if (list->last)
        list->last = list->last->prev;
    free(old);
    --list->length;
}

static void list_destroy(list_t *list) {
    while (list->length > 0)
        list_pop_back(list);
    free(list);
}

int main() {
    list_t *list = list_create();

    list_add(list, (void *)"1");
    list_add(list, (void *)"2");
    list_add(list, (void *)"3");

    list_node_t *ptr = list->first;
    while (ptr) {
        printf("Node: %s\n", (const char *)ptr->value);
        ptr = ptr->next;
    }
    putchar('\n');
    ptr = list->last;
    while (ptr) {
        printf("Node: %s\n", (const char *)ptr->value);
        ptr = ptr->prev;
    }

    list_destroy(list);
}
