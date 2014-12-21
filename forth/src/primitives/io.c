#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

state_t __DOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num))
    {
        int base = get_variable(ctx, BASE, DEFAULT_BASE).val;
        printnum(num, base);
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
    if (popnum(ctx->ds, &num))
    {
        int base = get_variable(ctx, BASE, DEFAULT_BASE).val;
        printnum(num < 0 ? num + 0x80000000 : num, base);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __DOT_S(context_t *ctx)
{
    int base = get_variable(ctx, BASE, DEFAULT_BASE).val;

    dlist_elem_t *element = dlist_tail(ctx->ds);
    while (element != NULL)
    {
        int *num = dlist_data(element);

        printnum(*num, base);
        element = dlist_prev(element);
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



void init_io_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, ".",      __DOT,    "( n -- )", "convert signed number n to string of digits, and output.");
    add_primitive(htbl, ".S",     __DOT_S,  "( -- )", "display stack contents.");
    add_primitive(htbl, "EMIT",   __EMIT,   "( ascii -- )", "outputs ascii as character.");
    add_primitive(htbl, "KEY",    __KEY,    "( -- ascii )", "waits for key, returns ascii.");
    add_primitive(htbl, "CR",     __CR,     "( -- )", "outputs a line break.");
    add_primitive(htbl, "SPACE",  __SPACE,  "( -- )", "outputs one single space character.");
    add_primitive(htbl, "SPACES", __SPACES, "( u -- )", "outputs u space characters.");
    add_primitive(htbl, "PAGE",   __CLS,    "( -- )", "clear screen.");
    add_primitive(htbl, "CLS",    __CLS,    "( -- )", "clear screen.");
    add_primitive(htbl, "U.",     __UDOT,   "( u -- )", "convert unsigned number n to string of digits, and output.");

    add_variable(htbl, "BASE", comma(ctx, (word_t)10)); //__BASE,   "( -- a )", "a is the address of a cell containing the current number-conversion radix {{2...36}}.");
    //add_primitive(htbl, "DECIMAL",__DECIMAL,"( -- )", "Set contents of BASE to 10.");
    //add_primitive(htbl, "HEX",    __HEX,    "( -- )", "Set contents of BASE to sixteen.");
}
