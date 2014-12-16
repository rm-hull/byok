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

state_t __COMPILED_WORD_REF(context_t *ctx)
{
    // need to work out which word is being called and jump to it
    // set ctx->w from entry addr_t
}


int lookup_param(hashtable_t *htbl, char *name, word_t *word)
{
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
    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = stack_effect;
    entry->docstring = docstring;
    entry->code_ptr = code_ptr;
    entry->param.val = 0;

    return hashtable_insert(htbl, entry);
}

int add_variable(hashtable_t *htbl, char *name, addr_t addr)
{
    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.addr = addr;

    return hashtable_insert(htbl, entry);
}

int add_constant(hashtable_t *htbl, char *name, int value)
{
    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->len = strlen(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.val = value;

    return hashtable_insert(htbl, entry);
}

int add_word(hashtable_t *htbl, char *name, addr_t addr)
{

}


int find_entry(hashtable_t *htbl, char *name, entry_t **entry)
{
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
