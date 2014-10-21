#ifndef _ENTRY_H
#define _ENTRY_H 1

#include <collections/stack.h>
#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int len;
    char *word;
    char *spec;
    int (*exec)(context_t *ctx);
} entry_t;

extern int add_entry(hashtable_t *htbl, char *word, int (*fn)(context_t *ctx), char *spec);

extern int find_entry(hashtable_t *htbl, char *word, entry_t **entry);

extern int entry_hash(const void *data);

extern int entry_match(const void *data1, const void *data2);

#ifdef __cplusplus
}
#endif

#endif