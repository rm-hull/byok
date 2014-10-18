#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/system.h>     // TODO - move readline out of here

#include <collections/stack.h>
#include <collections/list.h>

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

int isnumber(char *s)
{
    // TODO: check cotains only digits
    return strlen(s) > 0;
}

void repl()
{
    stack_t *ds = malloc(sizeof(stack_t));
    stack_init(ds, free);

    char *input = (char *)malloc(READLINE_BUFSIZ);

    while (true)
    {
        show_prompt(0, stack_size(ds));
        readline(input, READLINE_BUFSIZ);
        char *token = strtok(input, DELIMITERS);
        while (token != NULL)
        {
            char *s = trim(strdup(token));

            // TODO: all this primitive stuff to go in a dispatch table
            if (memcmp(s, ".S", 3) == 0)
            {
                list_elem_t *le = list_head(ds);
                while (le != NULL)
                {
                    terminal_writestring(list_data(le));
                    terminal_putchar(' ');
                    le = list_next(le);
                }
            }
            else if (memcmp(s, ".", 2) == 0)
            {
                if (stack_empty(ds))
                {
                    char *data;
                    stack_pop(ds, (void **)&data);
                    terminal_writestring(data);
                    terminal_putchar(' ');
                    free(data);
                }
                else
                {
                    printf("ERROR: stack underflow");
                }
            }
            else if (isnumber(s))

            {
                char *word = strdup(s);
                stack_push(ds, word);
            }
            token = strtok(NULL, DELIMITERS);
            free(s);
        }
    }
}