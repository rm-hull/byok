#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <primitives.h>
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
    else if (ctx->state == OK)
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

    ctx->state = OK;
    ctx->ds = malloc(sizeof(stack_t));
    stack_init(ctx->ds, free);

    ctx->rs = malloc(sizeof(stack_t));
    stack_init(ctx->rs, free);

    ctx->exe_tok = malloc(sizeof(hashtable_t));
    hashtable_init(ctx->exe_tok, BUCKETS, entry_hash, entry_match, free);

    init_arithmetic_words(ctx);
    init_bit_logic_words(ctx);
    init_comparison_words(ctx);
    init_io_words(ctx);
    init_misc_words(ctx);
    init_stack_manipulation_words(ctx);
    init_memory_words(ctx);

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
    pushnum(ctx->ds, (int)ctx->ip);
    return OK;
}

void literal(context_t *ctx, int n)
{
    compile(ctx, 2, __DOLIT, n);
}


void repl()
{
    context_t *ctx = init_context();
    history_t *hist = init_history(READLINE_HISTSIZ);

    while (true)
    {
        prompt(ctx);
        readline(ctx->tib->buffer, READLINE_BUFSIZ, hist->items);
        add_history(hist, ctx->tib->buffer);

        char *inbuf = strdup(ctx->tib->buffer);

        ctx->tib->token = strtok(inbuf, DELIMITERS);

        while (ctx->tib->token != NULL)
        {
            ctx->tib->cur_offset = ctx->tib->token - inbuf;
            //printf("token = %s, cur_offset = %d\n", ctx->tib->token, ctx->tib->cur_offset);

            int num;
            char *s = trim(strdup(ctx->tib->token));
            assert(s != NULL);

            entry_t *entry;
            if (find_entry(ctx->exe_tok, s, &entry) == 0)
            {
                //printf("%s %s %x\n", entry->word, entry->spec, entry->exec);
                ctx->w = entry->param;
                if (!immediate_mode(entry) && ctx->state == SMUDGE)
                {
                    // TODO need to compile in the entry->param if code_ptr == __EXEC
                    compile(ctx, 1, entry->code_ptr);
                    if (param_follows(entry))
                    {
                        compile(ctx, 1, entry->param);
                    }
                }
                else
                {
                    ctx->state = entry->code_ptr(ctx);
                }
            }
            else
            {
                int base = get_variable(ctx, BASE, DEFAULT_BASE).val;
                if (parsenum(s, &num, base))
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
                    // TODO - use proper error codes
                    ctx->state = error(ctx, -99, "unknown word: '%s'", s);
                }
            }
            ctx->tib->token = strtok(NULL, DELIMITERS);
            free(s);

            if (ctx->state == ERROR) break;
        }
        free(inbuf);
    }
}