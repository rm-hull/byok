#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <stack_machine/common.h>
#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <collections/hashtable.h>

state_t __REF(context_t *ctx)
{
    pushnum(ctx->ds, ctx->w.val);
    return OK;
}

state_t __EXEC(context_t *ctx)
{
    state_t (*code_ptr)(context_t *ctx);
    state_t retval;

    code_ptr = (void *)ctx->mem[ctx->w.addr].addr;

    do
    {
        if (code_ptr == __EXEC || code_ptr == __REF)
        {
            ctx->w = ctx->mem[ctx->ip++];
        }

        retval = code_ptr(ctx);
        code_ptr = (void *)ctx->mem[ctx->ip++].addr;

    } while (!stack_empty(ctx->rs) && retval == OK);

    return retval;
}

int set_flags(hashtable_t *htbl, char *name, int flags)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry;
    if (find_entry(htbl, name, &entry) == 0)
    {
        entry->flags = flags;
        return 0;
    }
    else
    {
        return -1;
    }

}

int lookup_param(hashtable_t *htbl, char *name, word_t *word)
{
    assert(htbl != NULL);
    assert(name != NULL);
    assert(word != NULL);

    entry_t *entry;
    if (find_entry(htbl, name, &entry) == 0)
    {
        *word = entry->param;
        return 0;
    }
    else
    {
        return -1;
    }
}


int add_primitive(hashtable_t *htbl, char *name, state_t (*code_ptr)(context_t *ctx), char *stack_effect, char *docstring)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = stack_effect;
    entry->docstring = docstring;
    entry->code_ptr = code_ptr;
    entry->param.val = 0;
    entry->flags = 0;

    return hashtable_insert(htbl, entry);
}

int add_variable(hashtable_t *htbl, char *name, addr_t addr)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.addr = addr;
    entry->flags = PARAM_FOLLOWS;

    return hashtable_insert(htbl, entry);
}

int add_constant(hashtable_t *htbl, char *name, int value)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.val = value;
    entry->flags = PARAM_FOLLOWS;

    return hashtable_insert(htbl, entry);
}

// replaces any existing word already defined
int add_word(hashtable_t *htbl, char *name, addr_t addr)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);

    // dont care what the return status is:
    // if it existed it was deleted, if it didnt, fine: nothing to do
    hashtable_remove(htbl, (void **)&entry);

    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __EXEC;
    entry->param.addr = addr;
    entry->flags = PARAM_FOLLOWS;

    return hashtable_insert(htbl, entry);
}


int find_entry(hashtable_t *htbl, char *name, entry_t **entry)
{
    assert(htbl != NULL);
    assert(name != NULL);

    // Create a key filling in the name/length
    entry_t *data = malloc(sizeof(entry_t));
    if (data == NULL)
        return -1;

    data->name = strtoupper(strdup(name));
    data->len = strlen(data->name);

    entry_t *tmp = data;  // copy data ptr, so it can be freed later
    int retval;
    if ((retval = hashtable_lookup(htbl, (void **)&data)) == 0)
    {
        *entry = data;
    }

    free(tmp->name); // free'ing strdup
    free(tmp);

    return retval;
}

int entry_hash(const void *data)
{
    entry_t *entry = (entry_t *)data;
    return hashfn(entry->name);
}

int entry_match(const void *data1, const void *data2)
{
    entry_t *entry1 = (entry_t *)data1;
    entry_t *entry2 = (entry_t *)data2;

    // to match the key length of both entries must be the same
    if (entry1->len != entry2->len)
        return 0;

    // and their contents must be equal also
    return memcmp(entry1->name, entry2->name, entry1->len + 1) == 0;
}
