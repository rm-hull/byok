#include <stdlib.h>
#include <collections/stack.h>

int stack_push(stack_t *stack, const void *data)
{
    return dlist_ins_prev(stack, dlist_head(stack), data);
}

int stack_pop(stack_t *stack, void **data)
{
    return dlist_remove(stack, dlist_head(stack), data);
}
