#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <collections/stack.h>
#include <stack_machine/common.h>
#include <stack_machine/entry.h>

#define true 1
#define false 0

addr_t comma(context_t *ctx, int num)
{
    // TODO check if over limit
    ctx->mem[ctx->dp].val = num;
    return ctx->dp++;
}


// TODO: change int *num to word_t *num
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

int peeknum(stack_t *stack, int *num)
{
    if (stack_empty(stack))
        return false;

    int *data = stack_peek(stack);
    *num = *data;
    return true;
}

int pushnum(stack_t *stack, int num)
{
    int *n = malloc(sizeof(int));
    *n = num;
    stack_push(stack, n);
    return true;
}

word_t get_variable(context_t *ctx, char *name, int default_value)
{
    word_t var;
    if (lookup_param(ctx->exe_tok, name, &var) == 0)
    {
        return ctx->mem[var.addr];
    }
    else
    {
        return (word_t)default_value;
    }
}


int printnum(int num, int base)
{
    char *s = malloc(32);
    if (s == NULL)
        return false;

    itoa(num, s, base);
    printf("%s ", s);
    free((void *)s);
    return true;
}

int parsenum(char *s, int *num, int base)
{
    int i = atoi(s, base);
    int len = strlen(s) + 1;
    char *copy = calloc(0, len);
    itoa(i, copy, base);
    int retval = memcmp(s, copy, len);
    free((void *)copy);

    if (retval == 0)
    {
        *num = i;
        return true;
    }
    else
    {
        return false;
    }
}

int isnumber(char *s)
{
    if (*s == 0)
        return false;

    if (*s == '-')
        s++;

    char c;
    while ((c = *s++) != 0)
        if (!isdigit(c))
            return false;

    return true;
}
