#include <stdlib.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

int __DROP(context_t *ctx)
{
    int x;
    if (popnum(ctx->ds, &x))
    {
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __OVER(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1);
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x1);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __DUP(context_t *ctx)
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
        return stack_underflow();
    }
}


int __SWAP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        pushnum(ctx->ds, x1);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __NIP(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x2);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __ROT(context_t *ctx)
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
        return stack_underflow();
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
