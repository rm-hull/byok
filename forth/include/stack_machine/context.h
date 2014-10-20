#include <collections/stack.h>
#include <collections/hashtable.h>

typedef struct {
    int *mem;                   // memory
    unsigned int ip;            // instruction pointer
    int w;                      // word register

    stack_t *ds;                // data stack
    stack_t *rs;                // return stack
    stack_t *fs;                // float stack

    hashtable_t *exe_tok;       // execution tokens

    int errno;
    char *err_msg;
} context_t;