#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <stack_machine/common.h>
#include <stack_machine/compiler.h>
#include <stack_machine/entry.h>

/**
 * Allocate a word of space in memory, and advance DP by
 * the size of word (4 bytes).
 */
word_t *comma(context_t *ctx, word_t num)
{
    assert(ctx->dp - ctx->mem < MEMSIZ);
    *ctx->dp = num;
    return ctx->dp++;
}


void literal(context_t *ctx, int n)
{
    entry_t *entry;
    if (find_entry(ctx->exe_tok, "(LIT)", &entry) == 0)
    {
        compile(ctx, 2, entry, n);
    }
    else
    {
        assert(false); // Failed to find (LIT)
    }
}

void compile(context_t *ctx, int n, ...)
{
    va_list params;
    va_start(params, n);

    for (int i = 0; i < n; i++)
    {
        word_t arg = va_arg(params, word_t);
        comma(ctx, arg);
    }

    va_end(params);
}
