#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/system.h>     // TODO - move readline out of here

#define READLINE_BUFSIZ 256

void show_prompt(int stack_size)
{
    terminal_setcolor(0x0F);
    terminal_writestring("  ok");
    terminal_setcolor(0x07);
    for (int i = 0; i < stack_size; i++)
        terminal_putchar('.');

    terminal_putchar('\n');
    terminal_flush();
}

void repl()
{
    int stack_size = 0; // dummy
    char *input = (char *)malloc(READLINE_BUFSIZ);

    while (true)
    {
        show_prompt(stack_size);
        readline(input, READLINE_BUFSIZ);

        stack_size++;
    }
}