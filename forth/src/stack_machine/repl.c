#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <primitives.h>
#include <forth/resources.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

#include <util/history.h>
#include <collections/hashtable.h>
#include <collections/stack.h>
#include <collections/list.h>

#include <stack_machine/common.h>


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

state_t interpret(context_t *ctx, char *in);

context_t *init_context()
{
    context_t *ctx = malloc(sizeof(context_t));
    assert(ctx != NULL);

    ctx->mem = calloc(0, sizeof(byte_t) * 16384);
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
    char *buf = &system_forth;
    char *saveptr;
    char *line = strtok_r(buf, "\n", &saveptr);

    while (line != NULL)
    {
        interpret(ctx, line);
        line = strtok_r(NULL, "\n", &saveptr);
    }

    return ctx;
}

void mem_stress_test()
{
    void *x;
    int i = 1;
    while (true)
    {
        x = malloc(1024);
        printf("allocated %d bytes at: 0x%x\n", i * 1024, x);
        assert(x != NULL);
        i++;
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

state_t __DOLIT(context_t *ctx)
{
    pushnum(ctx->ds, (*(int *)(ctx->ip++)));
    return OK;
}

void literal(context_t *ctx, int n)
{
    compile(ctx, 2, __DOLIT, n);
}

state_t interpret(context_t *ctx, char *in)
{
    int n = strlen(in);
    if (n == 0)
    {
        return OK;
    }

    if (n >= READLINE_BUFSIZ)
    {
        return error_msg(ctx, -1, ": input larger than TIB size");
    }

    // Copy the input buffer into the context's TIB, then again for tokenizing
    memcpy(ctx->tib->buffer, in, n + 1);
    char *inbuf = strdup(ctx->tib->buffer);
    assert(inbuf != NULL);

    // Begin parsing tokens proper
    ctx->tib->token = strtok_r(inbuf, DELIMITERS, ctx->tib->saveptr);
    while (ctx->tib->token != NULL)
    {
        // Set the current offset
        ctx->tib->cur_offset = ctx->tib->token - inbuf;
        char *s = trim(strdup(ctx->tib->token));
        assert(s != NULL);

        // Is this a word already in the dictionary?
        entry_t *entry;
        if (find_entry(ctx->exe_tok, s, &entry) == 0)
        {
            // Word exists, so set the contents of the word register
            // to the dictionary param,
            ctx->w = entry->param;

            if (immediate_mode(entry) || ctx->state != SMUDGE)
            {
                // Execute immediately if word is marked as IMMEDIATE,
                // or not in compile mode
                int retval = entry->code_ptr(ctx);

                // Only propagte the state if an error has been signalled.
                // Certainly don't set to OK if in smudge mode.
                if (retval != OK || ctx->state != SMUDGE)
                    ctx->state = retval;
            }
            else
            {
                // Otherwise compile the word into the currently defined word
                // and if the word has a following parameter, supply that as well
                compile(ctx, 1, entry->code_ptr);
                if (param_follows(entry))
                    compile(ctx, 1, entry->param);
            }
        }
        else
        {
            int num;
            if (parsenum(s, &num, ctx->base))
            {
                if (ctx->state == SMUDGE)
                {
                    literal(ctx, num);
                }
                else if (pushnum(ctx->ds, num))
                {
                    ctx->state = OK;
                }
                else
                {
                    // ??stack overflow??
                }
            }
            else if (*s != '\0')
            {
                ctx->state = error_msg(ctx, -13, ": '%s'", s); // word not found
            }
        }
        ctx->tib->token = strtok_r(NULL, DELIMITERS, ctx->tib->saveptr);
        free(s);

        if (ctx->state == ERROR) break;
    }
    free(inbuf);

    return ctx->state;
}


void repl()
{
    context_t *ctx = init_context();
    history_t *hist = init_history(READLINE_HISTSIZ);
    char in[READLINE_BUFSIZ];

    while (true)
    {
        prompt(ctx);
        readline(in, READLINE_BUFSIZ, hist->items);
        add_history(hist, in);
        interpret(ctx, in);
    }
}