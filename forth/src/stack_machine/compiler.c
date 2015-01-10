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

context_t *load(context_t *ctx, char *filename, char *data)
{
    char *buf = strdup(data);
    char *saveptr;
    char *line = strtok_r(buf, "\n", &saveptr);
    int lineno = 1;

    while (line != NULL)
    {
        interpret(ctx, line);
        if (ctx->state == ERROR)
        {
            printf("  in '%s' at line %d:\n", filename, lineno);
            terminal_setcolor(0x0F);
            printf("%s\n", line);
            terminal_setcolor(0x07);
            break;
        }

        line = strtok_r(NULL, "\n", &saveptr);
        lineno++;
    }

    free(buf);
    return ctx;
}
