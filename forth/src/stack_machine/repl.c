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
    else if (ctx->state == OK)
    {
        terminal_setcolor(0x0F);
        terminal_writestring("  ok");
        terminal_setcolor(0x07);
        for (int i = 0, n = stack_size(ctx->ds); i < n; i++)
            terminal_putchar('.');

        terminal_putchar('\n');
    }
    else
    {
        ctx->state = OK;
    }
    terminal_flush();
}

context_t *init_context()
{
    context_t *ctx = calloc(0, sizeof(context_t));
    assert(ctx != NULL);

    ctx->mem = calloc(0, sizeof(byte_t) * MEMSIZ);
    assert(ctx->mem != NULL);

    ctx->dp = ctx->mem;
    ctx->ip = ctx->mem;

    ctx->tib = calloc(0, sizeof(inbuf_t));
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

    // Any word definitions from this point onwards are 'user-defined'
    ctx->sticky_flags = FLAG_USER_DEFINED;

    //load(ctx, "examples.fth", examples_forth);
    return ctx;
}

#define COMPLETER_SIZ 20
char *filtered_words[COMPLETER_SIZ];
char *filter_words(char *text, int state, context_t *ctx)
{
    if (state == 0)
    {
        char **words = get_words(ctx->exe_tok);
        char *match = strtoupper(strdup(text));
        int len = strlen(match);
        int n = ctx->exe_tok->size;

        memset(&filtered_words, 0, COMPLETER_SIZ);
        for (int i = 0, j = 0; i < n && j < COMPLETER_SIZ; i++)
        {
            if (memcmp(match, words[i], len) == 0)
            {
                filtered_words[j++] = words[i];
            }
        }

        free(words);
        free(match);
    }

    if (state == COMPLETER_SIZ)
    {
        return NULL;
    }
    else
    {
        return filtered_words[state];
    }
}

void repl()
{
    context_t *ctx = init_context();
    history_t *hist = init_history(READLINE_HISTSIZ);
    char in[READLINE_BUFSIZ];
    colorize_t colorizer = { .fn = &colorize, .free_vars = ctx };
    complete_t completer = { .fn = &filter_words, .free_vars = ctx };

    while (true)
    {
        prompt(ctx);

        // Only proceed with interpret if there is something in the buffer
        memset(in, 0, sizeof(in));
        while (strlen(in) == 0)
        {
            readline(in, READLINE_BUFSIZ, hist->items, &completer, &colorizer);
            trim(in);
        }

        add_history(hist, in);
        interpret(ctx, in);
    }
}