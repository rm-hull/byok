#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <collections/dlist.h>

typedef dlist_t stack_t;

#define stack_init dlist_init
#define stack_destroy dlist_destroy
#define stack_peek(stack) ((stack)->head == NULL ? NULL : (stack)->head->data)
#define stack_size dlist_size
#define stack_empty(stack) (stack_size(stack) == 0)

extern int stack_push(stack_t *stack, const void *data);
extern int stack_pop(stack_t *stack, void **data);

#endif
