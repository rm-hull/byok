#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

#include <util/license.h>

state_t __LICENSE(context_t *ctx)
{
    pager(license_text);
    return OK;
}



void init_misc_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "LICENSE", __LICENSE, "( -- )", "displays the MIT license text.");
}
