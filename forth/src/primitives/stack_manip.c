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

state_t __2DROP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __2SWAP(context_t *ctx)
{
    int x1, x2, x3, x4;
    if (popnum(ctx->ds, &x4) && popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x3);
        pushnum(ctx->ds, x4);
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


state_t __2DUP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1);
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

state_t __2OVER(context_t *ctx)
{
    int x1, x2, x3, x4;
    if (popnum(ctx->ds, &x4) && popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x3);
        pushnum(ctx->ds, x4);
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
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

state_t __RDEPTH(context_t *ctx)
{
    pushnum(ctx->ds, stack_size(ctx->rs));
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
        return error(ctx, -6);  // return stack underflow
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
        return error(ctx, -6);  // return stack underflow
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
        return error(ctx, -6);  // return stack underflow
    }
}


state_t __PICK(context_t *ctx)
{
    int u;
    if (popnum(ctx->ds, &u))
    {
        dlist_elem_t *element = dlist_head(ctx->ds);
        while (u >= 0 && element != NULL)
        {
            if (u == 0)
            {
                int *num = dlist_data(element);
                pushnum(ctx->ds, *num);
                return OK;
            }
            u--;
            element = dlist_next(element);
        }
    }

    return error(ctx, -11);  // result out of range
}


void init_stack_manipulation_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "DROP", __DROP, "( x -- )", "drop top stack element.");
    add_primitive(htbl, "SWAP", __SWAP, "( x1 x2 -- x2 x1)", "swap top two stack elements.");
    add_primitive(htbl, "OVER", __OVER, "( x1 x2 -- x1 x2 x1)", "copy NOS (next of stack) to top of stack.");
    add_primitive(htbl, "DUP",  __DUP,  "( x -- x x )", "duplicate top stack element.");
    add_primitive(htbl, "?DUP", __QDUP,  "( x -- 0 | x x )", "duplicate top stack element if it is non-zero.");
    add_primitive(htbl, "2DROP", __2DROP, "( x1 x2 -- )", "drop cell pair x1 x2 from the stack.");
    add_primitive(htbl, "2SWAP", __2SWAP, "( x1 x2 x3 x4 -- x3 x4 x2 x1)", "exchange the top two cell pairs.");
    add_primitive(htbl, "2OVER", __2OVER, "( x1 x2 x3 x4-- x1 x2 x3 x4 x1 x2)", "copy cell pai x1 x2 to the top of the stack.");
    add_primitive(htbl, "2DUP",  __2DUP,  "( x1 x2 -- x1 x2 x1 x2 )", "duplicate cell pair x1 x2.");
    add_primitive(htbl, "NIP",  __NIP,  "( x1 x2 -- x2 )", "remove NOS.");
    add_primitive(htbl, "TUCK", __TUCK, "( x1 x2 -- x2 x1 x2 )", "copy the first (top) stack item below the second stack item.");
    add_primitive(htbl, "ROT",  __ROT,  "( x1 x2 x3 -- x2 x3 x1 )", "rotate the top three stack entries.");
    add_primitive(htbl, "-ROT", __MINROT,  "( x1 x2 x3 -- x3 x1 x2 )", "rotate the top three stack entries.");
    add_primitive(htbl, "DEPTH", __DEPTH,  "( -- n )", "the number of single-cell values contained in the data stack before n was placed on the stack.");
    add_primitive(htbl, "RDEPTH", __RDEPTH,  "( -- n )", "the number of single-cell values contained in the return stack.");
    add_primitive(htbl, ">R", __TOR,  "( x -- )  ( R:  -- x)", "move x to the return stack.");
    add_primitive(htbl, "R>", __RFROM,  "( -- x ) ( R:  x -- )", "move x from the return stack to the data stack.");
    add_primitive(htbl, "R@", __RFETCH,  "( -- x ) ( R:  x -- x)", "copy x from the return stack to the data stack.");
    add_primitive(htbl, "RDROP", __RDROP,  "( -- ) ( R:  x -- )", "drop top return stack element.");
    add_primitive(htbl, "PICK", __PICK,  "( xu ... x1 x0 u -- xu ... x1 x0 xu )", "remove u. Copy the xu to the top of the stack.");
}
