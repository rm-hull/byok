#ifndef _PRIMITIVES_H
#define _PRIMITIVES_H 1

#include <collections/hashtable.h>
#include <stack_machine/context.h>


#define arity1stackop(name, op)                            \
    state_t name(context_t *ctx)                           \
    {                                                      \
        int x1;                                            \
        if (popnum(ctx->ds, &x1))                          \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return OK;                                     \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow(ctx);                   \
        }                                                  \
    }


#define arity2stackop(name, op)                            \
    state_t name(context_t *ctx)                           \
    {                                                      \
        int x1, x2;                                        \
        if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return OK;                                     \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow(ctx);                   \
        }                                                  \
    }


#define arity2stackop2(name, op1, op2)                     \
    state_t name(context_t *ctx)                           \
    {                                                      \
        int x1, x2;                                        \
        if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op1);                         \
            pushnum(ctx->ds, op2);                         \
            return OK;                                     \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow(ctx);                   \
        }                                                  \
    }


#define arity3stackop(name, op)                            \
    state_t name(context_t *ctx)                           \
    {                                                      \
        int x1, x2, x3;                                    \
        if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return OK;                                     \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow(ctx);                   \
        }                                                  \
    }

#define arity3stackop2(name, op1, op2)                     \
    state_t name(context_t *ctx)                           \
    {                                                      \
        int x1, x2, x3;                                    \
        if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op1);                         \
            pushnum(ctx->ds, op2);                         \
            return OK;                                     \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow(ctx);                   \
        }                                                  \
    }


extern void init_arithmetic_words(context_t *ctx);
extern void init_bit_logic_words(context_t *ctx);
extern void init_comparison_words(context_t *ctx);
extern void init_io_words(context_t *ctx);
extern void init_stack_manipulation_words(context_t *ctx);
extern void init_misc_words(context_t *ctx);
extern void init_memory_words(context_t *ctx);

#endif
