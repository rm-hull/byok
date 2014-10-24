#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

state_t __DOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        printnum(num, ctx->base);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __UDOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        printnum(num < 0 ? num + 0x80000000: num, ctx->base);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __DOT_S(context_t *ctx)
{
    // TODO - print in reverse order
    list_elem_t *le = list_head(ctx->ds);
    while (le != NULL)
    {
        int *num = list_data(le);
        printnum(*num, ctx->base);
        le = list_next(le);
    }

    return OK;
}


state_t __EMIT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        putchar(num);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __KEY(context_t *ctx)
{
    pushnum(ctx->ds, getchar());
    return OK;
}


state_t __CR(context_t *ctx)
{
    putchar('\n');
    return OK;
}


state_t __SPACE(context_t *ctx)
{
    putchar(' ');
    return OK;
}


state_t __SPACES(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        for (int i = 0; i < num; i++)
            putchar(' ');

        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}



state_t __CLS(context_t *ctx)
{
    terminal_clear();
    return OK;
}


state_t __BASE(context_t *ctx)
{
    // TODO - rather than physical memory, use the ctx->mem virtual memory
    pushnum(ctx->ds, (int)&ctx->base);
    return OK;
}


state_t __DECIMAL(context_t *ctx)
{
    ctx->base = 10;
    return OK;
}


state_t __HEX(context_t *ctx)
{
    ctx->base = 16;
    return OK;
}


void init_io_words(hashtable_t *htbl)
{
    add_entry(htbl, ".",      __DOT,    "( n -- )", "convert signed number n to string of digits, and output.");
    add_entry(htbl, ".S",     __DOT_S,  "( -- )", "display stack contents.");
    add_entry(htbl, "EMIT",   __EMIT,   "( ascii -- )", "outputs ascii as character.");
    add_entry(htbl, "KEY",    __KEY,    "( -- ascii )", "waits for key, returns ascii.");
    add_entry(htbl, "CR",     __CR,     "( -- )", "outputs a line break.");
    add_entry(htbl, "SPACE",  __SPACE,  "( -- )", "outputs one single space character.");
    add_entry(htbl, "SPACES", __SPACES, "( u -- )", "outputs u space characters.");
    add_entry(htbl, "PAGE",   __CLS,    "( -- )", "clear screen.");
    add_entry(htbl, "CLS",    __CLS,    "( -- )", "clear screen.");
    add_entry(htbl, "U.",     __UDOT,   "( u -- )", "convert unsigned number n to string of digits, and output.");
    add_entry(htbl, "BASE",   __BASE,   "( -- a )", "a is the address of a cell containing the current number-conversion radix {{2...36}}.");
    add_entry(htbl, "DECIMAL",__DECIMAL,   "( -- )", "Set contents of BASE to 10.");
    add_entry(htbl, "HEX",    __HEX,   "( -- )", "Set contents of BASE to sixteen.");
}
