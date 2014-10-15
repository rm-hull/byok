#ifndef LIST_H
#define LIST_H

#include <stdio.h>

typedef struct list_elem_ {
    void *data;
    struct list_elem_ *next;
} list_elem_t;

typedef struct {
    unsigned int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    list_elem_t *head;
    list_elem_t *tail;
} list_t;

extern void list_init(list_t *list, void (*destroy)(void *data));
extern void list_destroy(list_t *list);
extern int list_ins_next(list_t *list, list_elem_t * element, const void *data);
extern int list_rem_next(list_t *list, list_elem_t * element, void **data);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(list, element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif
