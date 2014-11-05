#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <words.h>
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
#define DELIMITERS " \t\n"


void show_prompt(int state, int stack_size)
{
    if (state == COMPILING)
    {
        terminal_writestring("|  ");
    }
    else if (state == OK)
    {
        terminal_setcolor(0x0F);
        terminal_writestring("  ok");
        terminal_setcolor(0x07);
        for (int i = 0; i < stack_size; i++)
            terminal_putchar('.');

        terminal_putchar('\n');
    }
    terminal_flush();
}



context_t *init_context()
{
    context_t *ctx = malloc(sizeof(context_t));

    ctx->base= 10;
    ctx->state = OK;
    ctx->ds = malloc(sizeof(stack_t));
    stack_init(ctx->ds, free);

    ctx->rs = malloc(sizeof(stack_t));
    stack_init(ctx->rs, free);

    ctx->exe_tok = malloc(sizeof(hashtable_t));
    hashtable_init(ctx->exe_tok, BUCKETS, entry_hash, entry_match, free);

    init_arithmetic_words(ctx->exe_tok);
    init_bit_logic_words(ctx->exe_tok);
    init_comparison_words(ctx->exe_tok);
    init_io_words(ctx->exe_tok);
    init_misc_words(ctx->exe_tok);
    init_stack_manipulation_words(ctx->exe_tok);

    printf("                         \r");
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

void repl()
{
    context_t *ctx = init_context();
    char *input = (char *)malloc(READLINE_BUFSIZ);
    history_t *hist = init_history(READLINE_HISTSIZ);

    while (true)
    {
        show_prompt(ctx->state, stack_size(ctx->ds));
        readline(input, READLINE_BUFSIZ, hist->items);
        add_history(hist, input);

        char *token = strtok(input, DELIMITERS);

        ctx->state = NONE;
        while (token != NULL)
        {
            int num;
            char *s = trim(strdup(token));
            assert(s != NULL);

            entry_t *entry;

            if (find_entry(ctx->exe_tok, s, &entry) == 0)
            {
                //printf("%s %s %x\n", entry->word, entry->spec, entry->exec);
                ctx->state = entry->exec(ctx);
            }
            else if (parsenum(s, &num, ctx->base))
            {
                if (pushnum(ctx->ds, num))
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
                ctx->state = error(ctx, -99, "unknown word: '%s'", s);
            }

            token = strtok(NULL, DELIMITERS);
            free(s);

            if (ctx->state == ERROR) break;
        }
    }
}