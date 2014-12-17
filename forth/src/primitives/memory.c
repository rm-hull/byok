#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <primitives.h>
#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <stack_machine/error.h>

state_t __NEST(context_t *ctx)
{
    pushnum(ctx->rs, ctx->ip);
    ctx->ip = ++ctx->w.addr;
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

state_t __COLON(context_t *ctx)
{
    // Skip to next token
    ctx->inbuf->token = strtok(NULL, DELIMITERS);
    if (ctx->inbuf->token != NULL)
    {
        char *name = strdup(ctx->inbuf->token);
        add_word(ctx->exe_tok, name, comma(ctx, (unsigned int)&__NEST));
    }
    return SMUDGE;
}

state_t __SEMICOLON(context_t *ctx)
{
    comma(ctx, (unsigned int)&__UNNEST);
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
    int addr;
    if (popnum(ctx->ds, &addr))
    {
        pushnum(ctx->ds, ctx->mem[addr].val);
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
    if (popnum(ctx->ds, (int *)&addr) && popnum(ctx->ds, &x))
    {
        ctx->mem[addr].val = x;
        return OK;
    }
    else
    {
        return stack_underflow(ctx);
    }
}

state_t __VARIABLE(context_t *ctx)
{
    // Skip to next token
    ctx->inbuf->token = strtok(NULL, DELIMITERS);
    if (ctx->inbuf->token != NULL)
    {
        entry_t *entry;
        if (find_entry(ctx->exe_tok, ctx->inbuf->token, &entry) != 0)
        {
            char *name = strdup(ctx->inbuf->token);
            add_variable(ctx->exe_tok, name, comma(ctx, 0));
        }
    }
    return OK;
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
            entry_t *entry;
            if (find_entry(ctx->exe_tok, ctx->inbuf->token, &entry) != 0)
            {
                char *constant_name = strdup(ctx->inbuf->token);
                add_constant(ctx->exe_tok, constant_name, x);
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
void init_memory_words(context_t *ctx)
{
    hashtable_t *htbl = ctx->exe_tok;
    add_primitive(htbl, ",", __COMMA, "( x -- )","Reserve one cell of data space and store x in the cell.");
    add_primitive(htbl, "HERE", __HERE, "( -- addr )","addr is the data-space pointer.");
    add_primitive(htbl, ":", __COLON, "( C: \"<spaces>name\" -- colon-sys )", "Enter compilation state and start the current definition, producing colon-sys.");
    add_primitive(htbl, ";", __SEMICOLON, "( C: colon-sys -- )", "End the current definition, allow it to be found in the dictionary and enter interpretation state, consuming colon-sys.");
    //add_primitive(htbl, "SOURCE", __SOURCE, "( -- c-addr u )", "c-addr is the address of, and u is the number of characters in, the input buffer.");
    //add_primitive(htbl, ">IN", __TO_IN, "( -- a-addr )", "a-addr is the address of a cell containing the offset in characters from the start of the input buffer to the start of the parse area.");
    add_primitive(htbl, "!", __STORE, "( x a-addr -- )", "Store x at a-addr.");
    add_primitive(htbl, "@", __FETCH, "( a-addr -- x )", "x is the value stored at a-addr.");
    add_primitive(htbl, "VARIABLE", __VARIABLE, "( \"<spaces>name\" -- )", "Skip leading space delimiters. Parse name delimited by a space. Create a definition for name with the execution semantics: `name Execution: ( -- a-addr )`. Reserve one cell of data space at an aligned address.");
    add_primitive(htbl, "CONSTANT", __CONSTANT, "( x \"<spaces>name\" -- )", "Skip leading space delimiters. Parse name delimited by a space. Create a definition for name with the execution semantics: `name Execution: ( -- x )`, which places x on the stack.");

    set_flags(htbl, ";", IMMEDIATE);
}
