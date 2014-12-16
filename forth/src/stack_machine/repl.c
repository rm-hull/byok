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

#define BUCKETS 256
#define READLINE_BUFSIZ 256
#define READLINE_HISTSIZ 100


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

    ctx->mem = malloc(sizeof(addr_t) * 0x10000);
    ctx->dp = 0;
    ctx->ip = 0;

    ctx->inbuf = malloc(sizeof(inbuf_t));
    ctx->inbuf->buffer = malloc(READLINE_BUFSIZ);

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
        addr_t arg = va_arg(params, addr_t);
        comma(ctx, arg);
    }

    va_end(params);
}

state_t __DOLIT(context_t *ctx)
{
    pushnum(ctx->ds, ctx->mem[ctx->ip++].val);
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
        readline(ctx->inbuf->buffer, READLINE_BUFSIZ, hist->items);
        add_history(hist, ctx->inbuf->buffer);

        char *inbuf = strdup(ctx->inbuf->buffer);

        ctx->inbuf->token = strtok(inbuf, DELIMITERS);

        while (ctx->inbuf->token != NULL)
        {
            ctx->inbuf->cur_offset = ctx->inbuf->token - inbuf;
            //printf("token = %s, cur_offset = %d\n", ctx->inbuf->token, ctx->inbuf->cur_offset);

            int num;
            char *s = trim(strdup(ctx->inbuf->token));
            assert(s != NULL);

            entry_t *entry;
            if (find_entry(ctx->exe_tok, s, &entry) == 0)
            {
                //printf("%s %s %x\n", entry->word, entry->spec, entry->exec);
                ctx->w.val = entry->param.val;
                if (!immediate_mode(entry) && ctx->state == SMUDGE)
                {
                    compile(ctx, 1, entry->code_ptr);
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
            ctx->inbuf->token = strtok(NULL, DELIMITERS);
            free(s);

            if (ctx->state == ERROR) break;
        }
        free(inbuf);
    }
}