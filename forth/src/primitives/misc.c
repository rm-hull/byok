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

state_t __DUMP(context_t *ctx)
{
    int size;
    word_t addr;
    if (popnum(ctx->ds, &size) && popnum(ctx->ds, (int *)&addr))
    {
        if (addr.val >= 0 && size > 0)
        {
            int columns = 1;
            char **text = dump((char *)addr.val, size, columns);
            pager(text);
            free(text);
        }
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


void init_misc_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, "LICENSE", __LICENSE, "( -- )", "displays the MIT license text.");
    add_primitive(htbl, "DUMP", __DUMP, "( n addr -- )", "Dumps n bytes starting from addr.");
}
