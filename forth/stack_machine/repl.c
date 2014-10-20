#include <stdlib.h>
#include <stdarg.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
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
    if (*s == 0) return false;

    char c;
    while ((c = *s++) != 0)
        if (!isdigit(c))
            return false;

    return true;
}

int popnum(stack_t *stack, int *num)
{
    if (stack_empty(stack))
        return false;

    int *data;
    stack_pop(stack, (void **)&data);
    *num = *data;
    free(data);
    return true;
}

int pushnum(stack_t *stack, int num)
{
    int *n = malloc(sizeof(int));
    *n = num;
    stack_push(stack, n);
    return true;
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
                    int *num = list_data(le);
                    printf("%d ", *num);
                    le = list_next(le);
                }
            }
            else if (memcmp(s, ".", 2) == 0)
            {
                int num;
                if (popnum(ds, &num)) {
                    printf("%d ", num);
                }
                else
                {
                    error("stack underflow");
                }
            }
            else if (memcmp(s, "+", 2) == 0)
            {
                int num1, num2;
                if (popnum(ds, &num1) && popnum(ds, &num2))
                {
                    pushnum(ds, num1 + num2);
                }
                else
                {
                    error("stack underflow");
                }
            }
            else if (memcmp(s, "DUP", 4) == 0)
            {
                int num;
                if (popnum(ds, &num))
                {
                    pushnum(ds, num);
                    pushnum(ds, num);
                }
                else
                {
                    error("stack underflow");
                }
            }
            else if (isnumber(s))
            {
                pushnum(ds, atoi(s));
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