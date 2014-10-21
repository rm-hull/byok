#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <collections/hashtable.h>
#include <collections/stack.h>
#include <collections/list.h>

#define BUCKETS 256
#define READLINE_BUFSIZ 256
#define DELIMITERS " \t\n"

void show_prompt(int state, int stack_size)
{
    if (state)
    {
        terminal_writestring("|  ");
    }
    else
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

void error(char *msg, ...)
{
    va_list params;
    va_start(params, msg);

    terminal_setcolor(COLOR_LIGHT_RED);
    terminal_writestring("ERROR: ");
    terminal_setcolor(COLOR_LIGHT_GREY);

    vprintf(msg, params);
    va_end(params);
}

int isnumber(char *s)
{
    if (*s == 0)
        return false;

    if (*s == '-')
        s++;

    char c;
    while ((c = *s++) != 0)
        if (!isdigit(c))
            return false;

    return true;
}

// TODO - move to header
extern void init_core_words(hashtable_t *htbl);

context_t *init_context()
{
    context_t *ctx = malloc(sizeof(context_t));

    ctx->ds = malloc(sizeof(stack_t));
    stack_init(ctx->ds, free);

    ctx->exe_tok = malloc(sizeof(hashtable_t));
    hashtable_init(ctx->exe_tok, BUCKETS, entry_hash, entry_match, free);

    init_core_words(ctx->exe_tok);
    printf("                         \r");
    return ctx;
}

void repl()
{
    context_t *ctx = init_context();
    char *input = (char *)malloc(READLINE_BUFSIZ);

    while (true)
    {
        show_prompt(0, stack_size(ctx->ds));
        readline(input, READLINE_BUFSIZ);
        char *token = strtok(input, DELIMITERS);
        while (token != NULL)
        {
            char *s = trim(strdup(token));
            entry_t *entry;

            if (find_entry(ctx->exe_tok, s, &entry) == 0)
            {
                //printf("%s %s %x\n", entry->word, entry->spec, entry->exec);
                entry->exec(ctx);
            }
            else if (isnumber(s))
            {
                pushnum(ctx->ds, atoi(s));
            }
            else if (*s != '\0')
            {
                error("unknown word: '%s'", s);
            }

            token = strtok(NULL, DELIMITERS);
            free(s);
        }
    }
}