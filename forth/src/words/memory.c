#include <stdlib.h>
#include <stdio.h>

#include <words.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>
#include <stack_machine/vocab.h>


state_t __COLON(context_t *ctx)
{
    // TODO - enter compilation state
    return COMPILING;
}

state_t __SEMICOLON(context_t *ctx)
{
    // TODO - end compilation state
    return OK;
}

state_t __SOURCE(context_t *ctx)
{
   pushnum(ctx->ds, (int)&ctx->inbuf->buffer);
   pushnum(ctx->ds, strlen(ctx->inbuf->buffer));
   return OK;
}

state_t __TO_IN(context_t *ctx)
{
    pushnum(ctx->ds, (int)ctx->inbuf->buffer + ctx->inbuf->cur_offset);
    return OK;
}

arity1stackop(__FETCH, *((int *)x1))

state_t __STORE(context_t *ctx)
{
    int x, a_addr;
    if (popnum(ctx->ds, &a_addr) && popnum(ctx->ds, &x))
    {
        *((int *)a_addr) = x;
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __VARIABLE_REF(context_t *ctx)
{
    char *variable_name = ctx->inbuf->token;
    vocab_t *vocab;
    if (find_vocab(ctx->vocab, variable_name, &vocab) == 0)
    {
        pushnum(ctx->ds, (int)&vocab->value);
        return OK;
    }
    else
    {
        // something went badly wrong
        assert(false);
    }
}


state_t __VARIABLE(context_t *ctx)
{
    // Skip to next token
    ctx->inbuf->token = strtok(NULL, DELIMITERS);
    if (ctx->inbuf->token != NULL)
    {
        vocab_t *vocab;

        if (find_vocab(ctx->vocab, ctx->inbuf->token, &vocab) != 0)
        {
            char *variable_name = strdup(ctx->inbuf->token);
            add_vocab(ctx->vocab, variable_name);
            add_entry(ctx->exe_tok, variable_name, __VARIABLE_REF, NULL, NULL);
        }
    }
    return OK;
}


state_t __WORD(context_t *ctx)
{
    int ch;
    if (popnum(ctx->ds, &ch))
    {
        ctx->inbuf->token = strtok(NULL, DELIMITERS);
        if (ctx->inbuf->token != NULL)
        {
            printf("word token = %s\n", ctx->inbuf->token);
        }

        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

void init_memory_words(hashtable_t *htbl)
{
    add_entry(htbl, ":", __COLON, "( C: \"<spaces>name\" -- colon-sys )", "Enter compilation state and start the current definition, producing colon-sys.");
    add_entry(htbl, ";", __SEMICOLON, "( C: colon-sys -- )", "End the current definition, allow it to be found in the dictionary and enter interpretation state, consuming colon-sys.");
    add_entry(htbl, "SOURCE", __SOURCE, "( -- c-addr u )", "c-addr is the address of, and u is the number of characters in, the input buffer.");
    add_entry(htbl, ">IN", __TO_IN, "( -- a-addr )", "a-addr is the address of a cell containing the offset in characters from the start of the input buffer to the start of the parse area.");
    add_entry(htbl, "!", __STORE, "( x a-addr -- )", "Store x at a-addr.");
    add_entry(htbl, "@", __FETCH, "( a-addr -- x )", "x is the value stored at a-addr.");
    add_entry(htbl, "VARIABLE", __VARIABLE, "( \"<spaces>name\" -- )", "Skip leading space delimiters. Parse name delimited by a space. Create a definition for name with the execution semantics: `name Execution: ( -- a-addr)`. Reserve one cell of data space at an aligned address.");
}
