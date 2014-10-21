#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <stack_machine/context.h>
#include <stack_machine/entry.h>
#include <collections/hashtable.h>

char *strtoupper(char *s)
{
    for (int i = 0; s[i] != 0; i++)
        s[i] = toupper(s[i]);

    return s;
}

int add_entry(hashtable_t *htbl, char *word, int (*fn)(context_t *ctx), char *spec)
{
    entry_t *entry = malloc(sizeof(entry_t));
    if (entry == NULL)
        return -1;

    entry->word = strtoupper(word);
    entry->len = strlen(word);
    entry->spec = spec;
    entry->exec = fn;

    printf("Compiling: %s     \r", entry->word);

    return hashtable_insert(htbl, entry);
}

int find_entry(hashtable_t *htbl, char *word, entry_t **entry)
{
    // Create a key filling in the word/length
    entry_t *data = malloc(sizeof(entry_t));
    if (data == NULL)
        return -1;

    data->word = strtoupper(strdup(word));
    data->len = strlen(data->word);
    //printf("data '%s' [before] = 0x%x, ", data->word, data);

    entry_t *tmp = data;  // copy data ptr, so it can be freed later
    int retval;
    if ((retval = hashtable_lookup(htbl, (void **)&data)) == 0)
    {
        //printf("find: %s %s %x\n", data->word, data->spec, data->exec);
        *entry = data;
    }

    //printf("[after] = 0x%x\n", data);
    free(tmp->word); // free'ing strdup
    free(tmp);

    return retval;
}

int entry_hash(const void *data)
{
    entry_t *entry = (entry_t *)data;
    return hashfn(entry->word);
}

int entry_match(const void *data1, const void *data2)
{
    entry_t *entry1 = (entry_t *)data1;
    entry_t *entry2 = (entry_t *)data2;

    // to match the key length of both entries must be the same
    if (entry1->len != entry2->len)
        return 0;

    // and their contents must be equal also
    return memcmp(entry1->word, entry2->word, entry1->len + 1) == 0;
}
