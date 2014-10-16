#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <collections/list.h>

typedef list_t stack_t;

#define stack_init list_init
#define stack_destroy list_destroy
#define stack_peek(stack) ((stack)->head == NULL ? NULL : (stack)->head->data)
#define stack_size list_size

extern int stack_push(stack_t *stack, const void *data);
extern int stack_pop(stack_t *stack, void **data);

#endif
