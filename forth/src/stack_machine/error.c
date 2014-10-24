#include <stdlib.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

#include <stack_machine/error.h>
#include <stack_machine/context.h>
#include <stack_machine/common.h>

state_t stack_abort(context_t *ctx)
{
    // drain the stack
    int num;
    while (popnum(ctx->ds, &num));
    return ERROR;
}

state_t error(context_t *ctx, int errno, char *msg, ...)
{

    va_list params;
    va_start(params, msg);

    terminal_setcolor(COLOR_LIGHT_RED);
    terminal_writestring("ERROR: ");
    terminal_setcolor(COLOR_LIGHT_GREY);

    vprintf(msg, params);
    va_end(params);

    terminal_putchar('\n');

    return stack_abort(ctx);
}

state_t stack_underflow(context_t *ctx)
{
    return error(ctx, -4, "stack underflow");
}