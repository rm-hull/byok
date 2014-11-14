#ifndef DLIST_H
#define DLIST_H

#include <stdio.h>

typedef struct dlist_elem_ {
    void *data;
    struct dlist_elem_ *prev;
    struct dlist_elem_ *next;
} dlist_elem_t;

typedef struct {
    unsigned int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    dlist_elem_t *head;
    dlist_elem_t *tail;
} dlist_t;

extern void dlist_init(dlist_t *list, void (*destroy)(void *data));
extern void dlist_destroy(dlist_t *list);
extern int dlist_ins_next(dlist_t *list, dlist_elem_t *element, const void *data);
extern int dlist_ins_prev(dlist_t *list, dlist_elem_t *element, const void *data);
extern int dlist_remove(dlist_t *list, dlist_elem_t *element, void **data);

#define dlist_size(list) ((list)->size)
#define dlist_head(list) ((list)->head)
#define dlist_tail(list) ((list)->tail)
#define dlist_is_head(list, element) ((element)->prev == NULL ? 1 : 0)
#define dlist_is_tail(list, element) ((element)->next == NULL ? 1 : 0)
#define dlist_data(element) ((element)->data)
#define dlist_next(element) ((element)->next)
#define dlist_prev(element) ((element)->prev)

#endif
