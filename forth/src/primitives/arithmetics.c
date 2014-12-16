#include <stdlib.h>
#include <math.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>


arity2stackop(__ADD, x1 + x2)
arity2stackop(__SUB, x1 - x2)
arity2stackop(__MUL, x1 * x2)
arity2stackop(__DIV, x1 / x2)

arity1stackop(__INC, x1 + 1)
arity1stackop(__DEC, x1 - 1)
arity1stackop(__DBL, x1 * 2)
arity1stackop(__HALF, x1 / 2)
arity1stackop(__ABS, x1 >= 0 ? x1 : -x1)
arity1stackop(__NEG, -x1)

arity2stackop(__MIN, min(x1, x2))
arity2stackop(__MAX, max(x1, x2))
arity2stackop(__MOD, x1 % x2)


void init_arithmetic_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "+",  __ADD,  "( x1 x2 -- x3 )", "adds x1 and x2, leaves result x3.");
    add_primitive(htbl, "-",  __SUB,  "( x1 x2 -- x3 )", "subtracts x2 from x1, leaves result x3.");
    add_primitive(htbl, "*",  __MUL,  "( x1 x2 -- x3 )", "multiplies x1 with x2, leaves result x3.");
    add_primitive(htbl, "/",  __DIV,  "( x1 x2 -- x3 )", "divides x1 by x2, leaves result x3.");
    add_primitive(htbl, "1+", __INC,  "( x1 -- x2 )", "increments x1 by 1.");
    add_primitive(htbl, "1-", __DEC,  "( x1 -- x2 )", "decrements x1 by 1.");
    add_primitive(htbl, "2*", __DBL,  "( x1 -- x2 )", "multiply x1 by 2.");
    add_primitive(htbl, "2/", __HALF, "( n1 -- n2 )", "divide n1 by 2.");
    add_primitive(htbl, "ABS", __ABS, "( n -- u )", "return absolute value of n.");
    add_primitive(htbl, "NEGATE", __NEG, "( n1 -- n2 )", "change sign of n1.");
    add_primitive(htbl, "MIN",  __MIN,  "( n1 n2 -- n3 )", "return the lesser of the two signed numbers n1 and n2.");
    add_primitive(htbl, "MAX",  __MAX,  "( n1 n2 -- n3 )", "return the greater of the two signed numbers n1 and n2.");
    add_primitive(htbl, "MOD",  __MOD,  "( n1 n2 -- n3 )", "calculates and returns remainder of division x1/x2.");
}
