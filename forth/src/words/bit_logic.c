#include <stdlib.h>
#include <math.h>

#include <words.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

arity2stackop(__AND, x1 & x2)
arity2stackop(__OR,  x1 | x2)
arity2stackop(__XOR, x1 ^ x2)
arity1stackop(__INVERT, ~x1)
arity2stackop(__LSHIFT, x1 << x2)
arity2stackop(__RSHIFT, x1 >> x2)

void init_bit_logic_words(hashtable_t *htbl)
{
    add_entry(htbl, "AND",    __AND,     "( x1 x2 -- x3 )", "bitwise and x1 with x2, return result x3.");
    add_entry(htbl, "OR",     __OR,      "( x1 x2 -- x3 )", "bitwise or x1 with x2, return result x3.");
    add_entry(htbl, "XOR",    __XOR,     "( x1 x2 -- x3 )", "bitwise exclusive-or x1 with x2, return result x3.");
    add_entry(htbl, "INVERT", __INVERT,  "( x1 -- x2 )",    "return the bitwise complement of x1.");
    add_entry(htbl, "LSHIFT", __LSHIFT,  "( u1 u2 -- u3 )", "logical shift left u1 by u2 bits.");
    add_entry(htbl, "RSHIFT", __RSHIFT,  "( u1 u2 -- u3 )", "logical shift right u1 by u2 bits.");
}
