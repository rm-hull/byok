#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

int __DOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        printf("%d ", num);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __UDOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        printf("%d ", (unsigned int)num);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __DOT_S(context_t *ctx)
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


int __EMIT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        putchar(num);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __KEY(context_t *ctx)
{
    pushnum(ctx->ds, getchar());
}


int __CR(context_t *ctx)
{
    putchar('\n');
    return true;
}


int __SPACE(context_t *ctx)
{
    putchar(' ');
    return true;
}


int __SPACES(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num)) {
        for (int i = 0; i < num; i++)
            putchar(' ');

        return true;
    }
    else
    {
        return stack_underflow();
    }
}



int __CLS(context_t *ctx)
{
    terminal_clear();
    return true;
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
}
