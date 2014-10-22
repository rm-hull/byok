#include <stdlib.h>

#include <stack_machine/common.h>
#include <collections/stack.h>

#define true 1
#define false 0

// TODO move to common words
int popnum(stack_t *stack, int *num)
{
    if (stack_empty(stack))
        return false;

    int *data;
    stack_pop(stack, (void **)&data);
    *num = *data;
    free(data);
    return true;
}

// TODO move to common words
int pushnum(stack_t *stack, int num)
{
    int *n = malloc(sizeof(int));
    *n = num;
    stack_push(stack, n);
    return true;
}

