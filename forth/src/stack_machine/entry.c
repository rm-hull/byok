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

void indent(context_t *ctx)
{
    for (int i = 0, n = stack_size(ctx->rs); i < n; i++)
        printf("  ");
}

state_t __EXEC(context_t *ctx)
{
    if (ctx->echo)
    {
        indent(ctx);
        printf("Calling: 0x%x: %s   (%d)\n", ctx->ip, ctx->current_xt->name, ctx->current_xt);
    }

    ctx->current_xt = (entry_t *)*ctx->w.ptr;
    for (;;)
    {

        if (ctx->current_xt->code_ptr == __EXEC)
        {
            if (ctx->echo)
            {
                indent(ctx);
                printf("Calling: 0x%x: %s   (%d)\n", ctx->ip, ctx->current_xt->name, ctx->current_xt);
            }
            ctx->current_xt = (entry_t *)*ctx->w.ptr;
        }

        if (ctx->echo)
        {
            indent(ctx);
            printf("  Executing: 0x%x: %s   (%d)\n", ctx->ip, ctx->current_xt->name, ctx->current_xt);
        }

        state_t retval = ctx->current_xt->code_ptr(ctx);
        if (stack_empty(ctx->rs) || retval == ERROR)
            return retval;

        ctx->current_xt = (entry_t *)(*(ctx->ip)).addr;
        ctx->w = ctx->current_xt->param;
        ctx->ip++;
    }
}

// TODO: this can be deleted once add_primitive converted to use ctx
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


// TODO : convert to ctx->exe_tok
int add_primitive(hashtable_t *htbl, char *name, state_t (*code_ptr)(context_t *ctx), char *stack_effect, char *docstring)
{
    assert(htbl != NULL);
    assert(name != NULL);

    entry_t *entry = calloc(0, sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->stack_effect = stack_effect;
    entry->docstring = docstring;
    entry->code_ptr = code_ptr;
    entry->param.ptr = NULL;
    entry->flags = FLAG_PRIMITIVE;
    entry->alloc_size = 0;

    // TODO : add this once ctx available
    // ctx->last_word = entry;
    return hashtable_insert(htbl, entry);
}

int add_variable(context_t *ctx, char *name, word_t *addr)
{
    assert(ctx != NULL);
    assert(name != NULL);

    entry_t *entry = calloc(0, sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.ptr = (void *)addr;
    entry->flags = ctx->sticky_flags;
    entry->alloc_size = 0;

    return hashtable_insert(ctx->exe_tok, entry);
}

int add_constant(context_t *ctx, char *name, const int value)
{
    assert(ctx != NULL);
    assert(name != NULL);

    entry_t *entry = calloc(0, sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);
    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __REF;
    entry->param.val = value;
    entry->flags = ctx->sticky_flags;
    entry->alloc_size = 0;

    return hashtable_insert(ctx->exe_tok, entry);
}

// replaces any existing word already defined
int add_word(context_t *ctx, char *name, word_t *addr)
{
    assert(ctx != NULL);
    assert(name != NULL);

    entry_t *entry = calloc(0, sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->name = strtoupper(name);

    // dont care what the return status is:
    // if it existed it was deleted, if it didnt, fine: nothing to do
    hashtable_remove(ctx->exe_tok, (void **)&entry);

    entry->stack_effect = NULL;
    entry->docstring = NULL;
    entry->code_ptr = __EXEC;
    entry->param.ptr = (void *)addr;
    entry->flags = ctx->sticky_flags;
    entry->alloc_size = 0;

    ctx->last_word = entry;
    return hashtable_insert(ctx->exe_tok, entry);
}


int find_entry(hashtable_t *htbl, char *name, entry_t **entry)
{
    assert(htbl != NULL);
    assert(name != NULL);

    // Create a key filling in the name
    entry_t *data = malloc(sizeof(entry_t));
    if (data == NULL)
        return -1;

    data->name = strtoupper(strdup(name));

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
    return abs(hashfn(entry->name));
}

int entry_match(const void *data1, const void *data2)
{
    entry_t *a = (entry_t *)data1;
    entry_t *b = (entry_t *)data2;

    return strcmp(a->name, b->name) == 0;
}

/**
 * Returns a sorted list of all words. It is the callers responsibility
 * to free the list (but not the list items).
 */
char **get_words(hashtable_t *htbl)
{
    int n = htbl->size;
    char **words = calloc(0, (n + 1) * sizeof(char **));
    assert(words != NULL);

    int i = 0;
    for (int bucket = 0; bucket < htbl->buckets; bucket++)
    {
        list_t *list = &htbl->table[bucket];
        if (list->size > 0)
        {
            list_elem_t *element = list_head(list);
            while (element != NULL)
            {
                entry_t *entry = list_data(element);
                words[i++] = entry->name;
                element = list_next(element);
            }
        }
    }

    // sanity check that the size is the same as the accumulated count
    assert(i == n);

    int word_comparator(const char **a, const char **b)
    {
        return strcmp(*a, *b);
    }

    qsort(words, n, sizeof(char **), word_comparator);
    return words;
}
