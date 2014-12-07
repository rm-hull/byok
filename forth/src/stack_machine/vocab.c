#include <stdio.h>
#include <string.h>

#include <stack_machine/vocab.h>
#include <collections/hashtable.h>

int add_vocab(hashtable_t *htbl, char *name, int value)
{
    vocab_t *vocab = malloc(sizeof(vocab_t));
    if (vocab == NULL)
        return -1;

    vocab->name = strtoupper(name);
    vocab->len = strlen(name);
    vocab->value = value;

    return hashtable_insert(htbl, vocab);
}

int find_vocab(hashtable_t *htbl, char *name, vocab_t **vocab)
{
    // Create a key filling in the name/length
    vocab_t *data = malloc(sizeof(vocab_t));
    if (data == NULL)
        return -1;

    data->name = strtoupper(strdup(name));
    data->len = strlen(data->name);
    //printf("data '%s' [before] = 0x%x, ", data->name, data);

    vocab_t *tmp = data;  // copy data ptr, so it can be freed later
    int retval;
    if ((retval = hashtable_lookup(htbl, (void **)&data)) == 0)
    {
        //printf("find: %s %d\n", data->name, data->value);
        *vocab = data;
    }

    //printf("[after] = 0x%x, retval = %d\n", data, retval);
    free(tmp->name); // free'ing strdup
    free(tmp);

    return retval;
}

int vocab_hash(const void *data)
{
    vocab_t *vocab = (vocab_t *)data;
    return hashfn(vocab->name);
}

int vocab_match(const void *data1, const void *data2)
{
    vocab_t *vocab1 = (vocab_t *)data1;
    vocab_t *vocab2 = (vocab_t *)data2;

    // to match the key length of both entries must be the same
    if (vocab1->len != vocab2->len)
        return 0;

    // and their contents must be equal also
    return memcmp(vocab1->name, vocab2->name, vocab1->len + 1) == 0;
}
