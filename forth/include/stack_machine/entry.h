#ifndef _ENTRY_H
#define _ENTRY_H 1

#include <collections/stack.h>
#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IMMEDIATE 0x01

#define immediate_mode(entry) ((entry->flags & IMMEDIATE) == IMMEDIATE)


typedef struct {
    int len;
    char *name;
    unsigned int flags;
    char *stack_effect;
    char *docstring;
    state_t (*code_ptr)(context_t *ctx);
    word_t param;
} entry_t;

extern int set_flags(hashtable_t *htbl, char *name, int flags);
extern int lookup_param(hashtable_t *htbl, char *name, word_t *word);
extern int add_primitive(hashtable_t *htbl, char *name, state_t (*code_ptr)(context_t *ctx), char *stack_effect, char *docstring);
extern int add_variable(hashtable_t *htbl, char *name, addr_t addr);
extern int add_constant(hashtable_t *htbl, char *name, int value);
extern int add_word(hashtable_t *htbl, char *name, addr_t addr);

extern int find_entry(hashtable_t *htbl, char *name, entry_t **entry);
extern int entry_hash(const void *data);
extern int entry_match(const void *data1, const void *data2);

#ifdef __cplusplus
}
#endif

#endif