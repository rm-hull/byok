#include <stdlib.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

state_t __DROP(context_t *ctx)
{
    int x;
    if (popnum(ctx->ds, &x))
    {
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __OVER(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __DUP(context_t *ctx)
{
    int x;
    if (peeknum(ctx->ds, &x))
    {
        pushnum(ctx->ds, x);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __QDUP(context_t *ctx)
{
    int x;
    if (peeknum(ctx->ds, &x))
    {
        if (x != 0) pushnum(ctx->ds, x);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __SWAP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __NIP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __TUCK(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __ROT(context_t *ctx)
{
    int x1, x2, x3;
    if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x3);
        pushnum(ctx->ds, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __MINROT(context_t *ctx)
{
    int x1, x2, x3;
    if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x3);
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __DEPTH(context_t *ctx)
{
    pushnum(ctx->ds, stack_size(ctx->ds));
    return OK;
}


state_t __TOR(context_t *ctx)
{
    int x1;
    if (popnum(ctx->ds, &x1))
    {
        pushnum(ctx->rs, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __RFROM(context_t *ctx)
{
    int x1;
    if (popnum(ctx->rs, &x1))
    {
        pushnum(ctx->ds, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __RFETCH(context_t *ctx)
{
    int x1;
    if (peeknum(ctx->rs, &x1))
    {
        pushnum(ctx->ds, x1);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __RDROP(context_t *ctx)
{
    int x;
    if (popnum(ctx->rs, &x))
    {
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}



void init_stack_manipulation_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "DROP", __DROP, "( x -- )", "drop top stack element.");
    add_primitive(htbl, "SWAP", __SWAP, "( x1 x2 -- x2 x1)", "swap top two stack elements.");
    add_primitive(htbl, "OVER", __OVER, "( x1 x2 -- x1 x2 x1)", "copy NOS (next of stack) to top of stack.");
    add_primitive(htbl, "DUP",  __DUP,  "( x -- x x )", "duplicate top stack element.");
    add_primitive(htbl, "?DUP", __QDUP,  "( x -- 0 | x x )", "duplicate top stack element if it is non-zero.");
    add_primitive(htbl, "NIP",  __NIP,  "( x1 x2 -- x2 )", "remove NOS.");
    add_primitive(htbl, "TUCK", __TUCK, "( x1 x2 -- x2 x1 x2 )", "copy the first (top) stack item below the second stack item.");
    add_primitive(htbl, "ROT",  __ROT,  "( x1 x2 x3 -- x2 x3 x1 )", "rotate the top three stack entries.");
    add_primitive(htbl, "-ROT", __MINROT,  "( x1 x2 x3 -- x3 x1 x2 )", "rotate the top three stack entries.");
    add_primitive(htbl, "DEPTH", __DEPTH,  "( -- n )", "the number of single-cell values contained in the data stack before n was placed on the stack.");
    add_primitive(htbl, ">R", __TOR,  "( x -- )  ( R:  -- x)", "move x to the return stack.");
    add_primitive(htbl, "R>", __RFROM,  "( -- x ) ( R:  x -- )", "move x from the return stack to the data stack.");
    add_primitive(htbl, "R@", __RFETCH,  "( -- x ) ( R:  x -- x)", "copy x from the return stack to the data stack.");
    add_primitive(htbl, "RDROP", __RDROP,  "( -- ) ( R:  x -- )", "drop top return stack element.");
}
