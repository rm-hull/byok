#include <stdlib.h>
#include <math.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

#define truth(x) ((x) ? -1 : 0)

arity2stackop(__EQ, truth(x1 == x2))
arity2stackop(__NEQ,  truth(x1 != x2))
arity2stackop(__LT, truth(x1 < x2))
arity2stackop(__GT, truth(x1 > x2))
arity2stackop(__ULT, truth((unsigned int)x1 < (unsigned int)x2))
arity2stackop(__UGT, truth((unsigned int)x1 > (unsigned int)x2))
arity1stackop(__ISNEG, truth(x1 < 0))
arity1stackop(__ISZERO, truth(x1 == 0))
arity1stackop(__ISNOTZERO, truth(x1 != 0))
arity1stackop(__ISPOS, truth(x1 > 0))
arity3stackop(__WITHIN, truth(x1 >= x2 && x1 < x3))

void init_comparison_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "=",  __EQ,   "( x1 x2 -- f )", "compares top two stack elements, returns true flag if equal, false otherwise.");
    add_primitive(htbl, "<>", __NEQ,  "( x1 x2 -- f )", "compares top two stack elements, returns true flag if different, false otherwise.");
    add_primitive(htbl, "<",  __LT,   "( n1 n2 -- f )", "compares signed numbers n1 with n2, returns true if n1 is less then n2.");
    add_primitive(htbl, ">",  __GT,   "( n1 n2 -- f )", "compares signed numbers n1 with n2, returns true if n1 is greater then n2.");
    add_primitive(htbl, "U<", __ULT,  "( u1 u2 -- f )", "compares unsigned numbers u1 with u2, returns true if n1 is lower then n2.");
    add_primitive(htbl, "U>", __UGT,  "( u1 u2 -- f )", "compares unsigned numbers u1 with u2, returns true if n1 is higher then n2.");
    add_primitive(htbl, "0<", __ISNEG, "( n -- f )", "return a true flag if value of n is negative.");
    add_primitive(htbl, "0=", __ISZERO, "( x -- f )", "return a true flag if value of x is zero.");
    add_primitive(htbl, "0<>", __ISNOTZERO, "( x -- f )", "return a true flag if value of x is not zero.");
    add_primitive(htbl, "0>", __ISPOS,  "( n -- f )", "return a true flag if value of x is greater than zero.");
    add_primitive(htbl, "WITHIN", __WITHIN, "( x1 x2 x3 -- f )", "return a true flag if x1 is in the range of x2 ... x3-1.");

    // TODO: Move into system.fth
    add_constant(ctx, "FALSE", 0);
    add_constant(ctx, "TRUE", -1);
}
