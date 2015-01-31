#ifndef _ENTRY_H
#define _ENTRY_H 1

#include <collections/stack.h>
#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

// Flags
#define FLAG_IMMEDIATE      (1<<0)
#define FLAG_USER_DEFINED   (1<<1)
#define FLAG_PRIMITIVE      (1<<2)
#define FLAG_HIDDEN         (1<<3)
#define FLAG_CONSTANT       (1<<4)
#define FLAG_VARIABLE       (1<<5)


#define is_set(entry, f) ((entry->flags & f) == f)

extern void indent(context_t *ctx);
extern int set_flags(hashtable_t *htbl, char *name, int flags);
extern int add_primitive(hashtable_t *htbl, char *name, state_t (*code_ptr)(context_t *ctx), char *stack_effect, char *docstring);
extern int add_variable(context_t *ctx, char *name, word_t *addr);
extern int add_constant(context_t *ctx, char *name, const int value);
extern int add_word(context_t *ctx, char *name, word_t *addr);

extern int find_entry(hashtable_t *htbl, char *name, entry_t **entry);
extern int entry_hash(const void *data);
extern int entry_match(const void *data1, const void *data2);
extern char **get_words(hashtable_t *htbl);

#ifdef __cplusplus
}
#endif

#endif