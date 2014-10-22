#include <stdlib.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>


int __ADD(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1 + x2);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}



int __SUBTRACT(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1 - x2);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}



int __MULTIPLY(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1 * x2);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


int __DIVIDE(context_t *ctx)
{
    int x1, x2;
    if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))
    {
        pushnum(ctx->ds, x1 / x2);
        return true;
    }
    else
    {
        return stack_underflow();
    }
}


void init_arithmetic_words(hashtable_t *htbl)
{
    add_entry(htbl, "+", __ADD, "( x1 x2 -- x3 )", "adds x1 and x2, leaves result x3");
    add_entry(htbl, "-", __SUBTRACT, "( x1 x2 -- x3 )", "subtracts x2 from x1, leaves result x3");
    add_entry(htbl, "*", __MULTIPLY, "( x1 x2 -- x3 )", "multiplies x1 with x2, leaves result x3");
    add_entry(htbl, "/", __DIVIDE, "( x1 x2 -- x3 )", "divides x1 by x2, leaves result x3");
}
