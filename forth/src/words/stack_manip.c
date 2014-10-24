#include <stdlib.h>

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
    if (popnum(ctx->ds, &x))
    {
        pushnum(ctx->ds, x);
        pushnum(ctx->ds, x);
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

void init_stack_manipulation_words(hashtable_t *htbl)
{
    add_entry(htbl, "DROP", __DROP, "( x -- )", "drop top stack element.");
    add_entry(htbl, "SWAP", __SWAP, "( x1 x2 -- x2 x1)", "swap top two stack elements.");
    add_entry(htbl, "OVER", __OVER, "( x1 x2 -- x1 x2 x1)", "copy NOS (next of stack) to top of stack.");
    add_entry(htbl, "DUP",  __DUP,  "( x -- x x )", "duplicate top stack element.");
    add_entry(htbl, "NIP",  __NIP,  "( x1 x2 -- x2 )", "remove NOS.");
    add_entry(htbl, "ROT",  __ROT,  "( x1 x2 x3 -- x2 x3 x1 )", "TODO");
}
