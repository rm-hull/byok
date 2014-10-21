#include <stdlib.h>
#include <string.h>
#include <collections/hashtable.h>

int hashfn(const void *key)
{
    const char *ptr = (const char *)key;
    int tmp, val = 0;

    while (*ptr != '\0')
    {
        val = (val << 4) + (*ptr);
        if ((tmp = (val & 0xf0000000)))
        {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }

        ptr++;
    }

    return val;
}

int hashtable_init(hashtable_t *htbl,
                   int buckets,
                   int (*hash)(const void *key),
                   int (*match)(const void *key1, const void *key2),
                   void (*destroy)(void *data))
{
    if ((htbl->table = (list_t *)malloc(buckets * sizeof(list_t))) == NULL)
        return -1;

    htbl->buckets = buckets;
    for (int i = 0; i < htbl->buckets; i++)
        list_init(&htbl->table[i], destroy);

    htbl->hash = hash;
    htbl->match = match;
    htbl->destroy = destroy;
    htbl->size = 0;
    return 0;
}

void hashtable_destroy(hashtable_t *htbl)
{
    for (int i = 0; i < htbl->buckets; i++)
        list_destroy(&htbl->table[i]);

    free(htbl->table);
    memset(htbl, 0, sizeof(hashtable_t));
}

int hashtable_insert(hashtable_t *htbl, const void *data)
{
    void *temp;
    int bucket, retval;

    temp = (void *)data;
    if (hashtable_lookup(htbl, &temp) == 0)
        return 1;   // do nothing if data already in table

    bucket = htbl->hash(data) % htbl->buckets;
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
        htbl->size++;

    return retval;
}

int hashtable_remove(hashtable_t *htbl, void **data)
{
    list_elem_t *element, *prev;
    int bucket = htbl->hash(*data) % htbl->buckets;
    prev = NULL;

    for (element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element))
    {
        if (htbl->match(*data, list_data(element)))
        {
            // remove data from bucket
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0)
            {
                htbl->size--;
                return 0;
            }
            else
            {
                return -1;
            }
        }

        prev = element;
    }

    return -1;  // not found
}

int hashtable_lookup(const hashtable_t *htbl, void **data)
{
    list_elem_t *element;
    int bucket = htbl->hash(*data) % htbl->buckets;

    for (element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element))
    {
        if (htbl->match(*data, list_data(element)))
        {
            *data = list_data(element);     // pass back data from the table
            return 0;
        }
    }

    return -1;  // not found
}
