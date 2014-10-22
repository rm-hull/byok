#ifndef _WORDS_H
#define _WORDS_H 1

#include <collections/hashtable.h>


#define arity1stackop(name, op)                            \
    int name(context_t *ctx)                               \
    {                                                      \
        int x1;                                            \
        if (popnum(ctx->ds, &x1))                          \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return true;                                   \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow();                      \
        }                                                  \
    }


#define arity2stackop(name, op)                            \
    int name(context_t *ctx)                               \
    {                                                      \
        int x1, x2;                                        \
        if (popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return true;                                   \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow();                      \
        }                                                  \
    }


#define arity3stackop(name, op)                            \
    int name(context_t *ctx)                               \
    {                                                      \
        int x1, x2, x3;                                    \
        if (popnum(ctx->ds, &x3) && popnum(ctx->ds, &x2) && popnum(ctx->ds, &x1))  \
        {                                                  \
            pushnum(ctx->ds, op);                          \
            return true;                                   \
        }                                                  \
        else                                               \
        {                                                  \
            return stack_underflow();                      \
        }                                                  \
    }

extern void init_arithmetic_words(hashtable_t *htbl);
extern void init_bit_logic_words(hashtable_t *htbl);
extern void init_comparison_words(hashtable_t *htbl);
extern void init_io_words(hashtable_t *htbl);
extern void init_stack_manipulation_words(hashtable_t *htbl);

#endif
