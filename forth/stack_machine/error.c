#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

#include <stack_machine/error.h>

int error(char *msg, ...)
{
    va_list params;
    va_start(params, msg);

    terminal_setcolor(COLOR_LIGHT_RED);
    terminal_writestring("ERROR: ");
    terminal_setcolor(COLOR_LIGHT_GREY);

    vprintf(msg, params);
    va_end(params);

    return false;
}

int stack_underflow()
{
    return error("stack underflow");
}