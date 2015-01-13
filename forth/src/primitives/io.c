#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>
#include <stack_machine/compiler.h>
#include <stack_machine/slots.h>
#include <editor/editor.h>

state_t __DOT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num))
    {
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
    if (popnum(ctx->ds, &num))
    {
        printnum(num < 0 ? num + 0x80000000 : num, ctx->base);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __DOT_S(context_t *ctx)
{
    dlist_elem_t *element = dlist_tail(ctx->ds);
    while (element != NULL)
    {
        int *num = dlist_data(element);

        printnum(*num, ctx->base);
        element = dlist_prev(element);
    }

    return OK;
}


state_t __EMIT(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num))
    {
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


state_t __SPACES(context_t *ctx)
{
    int num;
    if (popnum(ctx->ds, &num))
    {
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


state_t __TYPE(context_t *ctx)
{
    addr_t addr;
    int num;
    if (popnum(ctx->ds, &num) && popnum(ctx->ds, (int)&addr))
    {
        for (int i = 0; i < num; i++)
            putchar(*((char*)addr++));

        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __LIST(context_t *ctx)
{
    int block;
    if (popnum(ctx->ds, &block))
    {
        char *data = slot_buffer(block);
        if (data != NULL)
        {
            screen_editor(ctx, block, data);
            return OK;
        }
        else
        {
            return error(ctx, -35);  // invalid block number
        }
    }

    return stack_underflow(ctx);
}

state_t __LOAD(context_t *ctx)
{
    int block;
    if (popnum(ctx->ds, &block))
    {
        char *data = slot_buffer(block);
        if (data != NULL)
        {
            load(ctx, "block #", data);
            return ctx->state;
        }
        else
        {
            return error(ctx, -35);  // invalid block number
        }
    }

    return stack_underflow(ctx);
}


state_t __CURSOR(context_t *ctx)
{

    unsigned int start;
    unsigned int end;

    if (popnum(ctx->ds, &end) && popnum(ctx->ds, &start))
    {
        terminal_cursormode(start, end);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

void init_io_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, ".",      __DOT,    "( n -- )", "convert signed number n to string of digits, and output.");
    add_primitive(htbl, ".S",     __DOT_S,  "( -- )", "display stack contents.");
    add_primitive(htbl, "EMIT",   __EMIT,   "( ascii -- )", "outputs ascii as character.");
    add_primitive(htbl, "KEY",    __KEY,    "( -- ascii )", "waits for key, returns ascii.");
    add_primitive(htbl, "SPACES", __SPACES, "( u -- )", "outputs u space characters.");
    add_primitive(htbl, "PAGE",   __CLS,    "( -- )", "clear screen.");
    add_primitive(htbl, "CLS",    __CLS,    "( -- )", "clear screen.");
    add_primitive(htbl, "U.",     __UDOT,   "( u -- )", "convert unsigned number n to string of digits, and output.");
    add_primitive(htbl, "TYPE",   __TYPE,   "( addr n -- )", "outputs the contents of addr for n bytes.");
    add_primitive(htbl, "LIST",   __LIST,   "( block -- )", "");
    add_primitive(htbl, "LOAD",   __LOAD,   "( block -- )", "");
    add_primitive(htbl, "CURSOR", __CURSOR, "( start end -- )", "");
}
