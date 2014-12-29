#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <collections/list.h>

#define abs(x) (x < 0 ? -x : x)

typedef struct {
    int buckets;
    int (*hash)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    int size;
    list_t *table;
} hashtable_t;

extern int hashfn(const void *key);

extern int hashtable_init(hashtable_t *htbl,
                          int buckets,
                          int (*hash)(const void *key),
                          int (*match)(const void *key1, const void *key2),
                          void (*destroy)(void *data));

extern void hashtable_destroy(hashtable_t *htbl);
extern int hashtable_insert(hashtable_t *htbl, const void *data);
extern int hashtable_remove(hashtable_t *htbl, void **data);
extern int hashtable_lookup(const hashtable_t *htbl, void **data);

#define hashtable_size(htbl) ((htbl)->size)

#endif
