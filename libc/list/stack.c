#include <stdlib.h>
#include <stack.h>

int stack_push(stack_t *stack, const void *data)
{
    return list_ins_next(stack, NULL, data);
}

int stack_pop(stack_t *stack, void **data)
{
    return list_rem_next(stack, NULL, data);
}
