#include <stdlib.h>

#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <collections/hashtable.h>

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

int dot(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        printf("%d ", num);
        return true;
    }
    else
    {
        error("stack underflow");
        return false;
    }
}

int dot_S(context_t *ctx)
{
    list_elem_t *le = list_head(ctx->ds);
    while (le != NULL)
    {
        int *num = list_data(le);
        printf("%d ", *num);
        le = list_next(le);
    }

    return true;
}


int ROT(context_t *ctx)
{
    int x1, x2, x3;
    if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x3);
        pushnum(ctx->ds, x1);
        return true;
    }
    else
    {
        error("stack underflow");
        return false;
    }
}


int DUP(context_t *ctx)
{
    int x;
    if (popnum(ctx->ds, &x))
    {
        pushnum(ctx->ds, x);
        pushnum(ctx->ds, x);
        return true;
    }
    else
    {
        error("stack underflow");
        return false;
    }
}

int PLUS(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1 + x2);
        return true;
    }
    else
    {
        error("stack underflow");
        return false;
    }
}


void init_core_words(hashtable_t *htbl)
{
    add_entry(htbl, "DUP", DUP, "( x -- x x )");
    add_entry(htbl, "ROT", ROT, "( x1 x2 x3 -- x2 x3 x1 )");
    add_entry(htbl, ".S", dot_S, "( -- )");
    add_entry(htbl, ".", dot, "( n -- )");
    add_entry(htbl, "+", PLUS, "( x1 x2 -- x1+x2 )");
}
