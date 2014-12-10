#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <words.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>
#include <stack_machine/vocab.h>

addr_t comma(context_t *ctx, int n)
{
    // TODO check if over limit
    ctx->mem[ctx->dp] = n;
    return ctx->dp++;
}

state_t __DOLIT(context_t *ctx)
{
    pushnum(ctx->ds, ctx->mem[ctx->ip++]);
    return OK;
}

state_t __NEST(context_t *ctx)
{
    pushnum(ctx->rs, ctx->ip);
    ctx->ip = ++ctx->w;
    return OK;
}

state_t __UNNEST(context_t *ctx)
{
    if (popnum(ctx->rs, (int *)&ctx->ip))
    {
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}


void compile(context_t *ctx, int n, ...)
{
    va_list params;
    va_start(params, n);

    for (int i = 0; i < n; i++)
    {
        addr_t arg = va_arg(params, addr_t);
        comma(ctx, arg);
    }

    va_end(params);
}

void literal(context_t *ctx, int n)
{
    compile(ctx, 2, __DOLIT, n);
}

state_t __COMMA(context_t *ctx)
{
    int n;
    if (popnum(ctx->ds, &n))
    {
        comma(ctx, n);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __HERE(context_t *ctx)
{
    pushnum(ctx->ds, ctx->dp);
    return OK;
}

state_t __COMPILED_WORD_REF(context_t *ctx)
{
    // need to work out which word is being called and jump to it
    // set ctx->w from entry addr_t
}


state_t __COLON(context_t *ctx)
{
    // Skip to next token
    ctx->inbuf->token = strtok(NULL, DELIMITERS);
    if (ctx->inbuf->token != NULL)
    {
        vocab_t *vocab;
        if (find_vocab(ctx->vocab, ctx->inbuf->token, &vocab) != 0)
        {
            char *word_name = strdup(ctx->inbuf->token);

            add_vocab(ctx->vocab, word_name, comma(ctx, (addr_t)&__NEST));
            add_entry(ctx->exe_tok, word_name, __COMPILED_WORD_REF, NULL, NULL);
        }
    }
    return SMUDGE;
}


state_t __SEMICOLON(context_t *ctx)
{
    comma(ctx, &__UNNEST);
    return OK;
}

/*
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
*/

state_t __FETCH(context_t *ctx)
{
    addr_t addr;
    if (popnum(ctx->ds, &addr))
    {
        pushnum(ctx->ds, ctx->mem[addr]);
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __STORE(context_t *ctx)
{
    int x;
    addr_t addr;
    if (popnum(ctx->ds, &addr) && popnum(ctx->ds, &x))
    {
        ctx->mem[addr] = x;
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
        pushnum(ctx->ds, vocab->addr);
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

            add_vocab(ctx->vocab, variable_name, comma(ctx, 0));
            add_entry(ctx->exe_tok, variable_name, __VARIABLE_REF, NULL, NULL);
        }
    }
    return OK;
}


state_t __CONSTANT_REF(context_t *ctx)
{
    char *constant_name = ctx->inbuf->token;
    vocab_t *vocab;
    if (find_vocab(ctx->vocab, constant_name, &vocab) == 0)
    {
        pushnum(ctx->ds, ctx->mem[vocab->addr]);
        return OK;
    }
    else
    {
        // something went badly wrong
        assert(false);
    }
}


state_t __CONSTANT(context_t *ctx)
{
    int x;
    if (popnum(ctx->ds, &x))
    {
        // Skip to next token
        ctx->inbuf->token = strtok(NULL, DELIMITERS);
        if (ctx->inbuf->token != NULL)
        {
            vocab_t *vocab;
            if (find_vocab(ctx->vocab, ctx->inbuf->token, &vocab) != 0)
            {
                char *constant_name = strdup(ctx->inbuf->token);
                add_vocab(ctx->vocab, constant_name, comma(ctx, x));
                add_entry(ctx->exe_tok, constant_name, __CONSTANT_REF, NULL, NULL);
            }
        }
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

/*
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
*/
void init_memory_words(hashtable_t *htbl)
{
    add_entry(htbl, ",", __COMMA, "( x -- )","Reserve one cell of data space and store x in the cell.");
    add_entry(htbl, "HERE", __HERE, "( -- addr )","addr is the data-space pointer.");
    add_entry(htbl, ":", __COLON, "( C: \"<spaces>name\" -- colon-sys )", "Enter compilation state and start the current definition, producing colon-sys.");
    add_entry(htbl, ";", __SEMICOLON, "( C: colon-sys -- )", "End the current definition, allow it to be found in the dictionary and enter interpretation state, consuming colon-sys.");
    //add_entry(htbl, "SOURCE", __SOURCE, "( -- c-addr u )", "c-addr is the address of, and u is the number of characters in, the input buffer.");
    //add_entry(htbl, ">IN", __TO_IN, "( -- a-addr )", "a-addr is the address of a cell containing the offset in characters from the start of the input buffer to the start of the parse area.");
    add_entry(htbl, "!", __STORE, "( x a-addr -- )", "Store x at a-addr.");
    add_entry(htbl, "@", __FETCH, "( a-addr -- x )", "x is the value stored at a-addr.");
    add_entry(htbl, "VARIABLE", __VARIABLE, "( \"<spaces>name\" -- )", "Skip leading space delimiters. Parse name delimited by a space. Create a definition for name with the execution semantics: `name Execution: ( -- a-addr )`. Reserve one cell of data space at an aligned address.");
    add_entry(htbl, "CONSTANT", __CONSTANT, "( x \"<spaces>name\" -- )", "Skip leading space delimiters. Parse name delimited by a space. Create a definition for name with the execution semantics: `name Execution: ( -- x )`, which places x on the stack.");
}
