#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <primitives.h>
#include <forth/resources.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/compiler.h>
#include <stack_machine/interpreter.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

#include <util/history.h>
#include <collections/hashtable.h>
#include <collections/stack.h>
#include <collections/list.h>

#include <editor/colorize.h>

void prompt(context_t *ctx)
{
    if (ctx->state == SMUDGE)
    {
        terminal_writestring("|  ");
    }
    else if (ctx->state == OK || ctx->state == SMUDGE_OK)
    {
        terminal_setcolor(0x0F);
        terminal_writestring("  ok");
        terminal_setcolor(0x07);
        for (int i = 0, n = stack_size(ctx->ds); i < n; i++)
            terminal_putchar('.');

        terminal_putchar('\n');
        ctx->state = NONE;
    }
    terminal_flush();
}

context_t *load(context_t *ctx, char *filename, char *buf)
{
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

    return ctx;
}

context_t *init_context()
{
    context_t *ctx = calloc(0, sizeof(context_t));
    assert(ctx != NULL);

    ctx->mem = calloc(0, sizeof(byte_t) * MEMSIZ);
    assert(ctx->mem != NULL);

    ctx->dp = ctx->mem;
    ctx->ip = ctx->mem;

    ctx->tib = malloc(sizeof(inbuf_t));
    ctx->tib->buffer = malloc(READLINE_BUFSIZ);

    ctx->base = DEFAULT_BASE;
    ctx->echo = DEFAULT_ECHO;
    ctx->state = OK;
    ctx->ds = malloc(sizeof(stack_t));
    stack_init(ctx->ds, free);

    ctx->rs = malloc(sizeof(stack_t));
    stack_init(ctx->rs, free);

    ctx->exe_tok = malloc(sizeof(hashtable_t));
    hashtable_init(ctx->exe_tok, BUCKETS, entry_hash, entry_match, free);

    // primitives
    init_arithmetic_words(ctx);
    init_bit_logic_words(ctx);
    init_comparison_words(ctx);
    init_io_words(ctx);
    init_misc_words(ctx);
    init_stack_manipulation_words(ctx);
    init_memory_words(ctx);

    // bootstrap forth system proper
    load(ctx, "system.fth", &system_forth);
//    load(ctx, "examples.fth", examples_forth);

    return ctx;
}

void repl()
{
    context_t *ctx = init_context();
    history_t *hist = init_history(READLINE_HISTSIZ);
    char in[READLINE_BUFSIZ];
    colorize_t colorizer = { .fn = &colorize, .free_vars = ctx };

    while (true)
    {
        prompt(ctx);
        readline(in, READLINE_BUFSIZ, hist->items, &colorizer);
        add_history(hist, in);
        interpret(ctx, in);
    }
}