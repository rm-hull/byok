#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <stack_machine/common.h>
#include <stack_machine/compiler.h>

state_t __DOLIT(context_t *ctx)
{
    pushnum(ctx->ds, (*(int *)ctx->ip++));
    return OK;
}

void literal(context_t *ctx, int n)
{
    static entry_t dolit = { .code_ptr = &__DOLIT, .name = "DOLIT" };
    compile(ctx, 2, &dolit, n);
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
