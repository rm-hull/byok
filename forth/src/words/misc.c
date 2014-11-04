#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

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



void init_misc_words(hashtable_t *htbl)
{
    add_entry(htbl, "LICENSE", __LICENSE, "( -- )", "displays the MIT license text.");
}
