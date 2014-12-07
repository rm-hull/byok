#ifndef _VOCAB_H
#define _VOCAB_H 1

#include <collections/stack.h>
#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *name;
    int  len;
    int  value;
} vocab_t;

extern int add_vocab(hashtable_t *htbl, char *name, int value);
extern int find_vocab(hashtable_t *htbl, char *name, vocab_t **vocab);
extern int vocab_hash(const void *data);
extern int vocab_match(const void *data1, const void *data2);

#ifdef __cplusplus
}
#endif

#endif