#ifndef _CONTEXT_H
#define _CONTEXT_H 1

#include <collections/stack.h>
#include <collections/hashtable.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NONE, OK, COMPILING, ERROR } state_t;

typedef struct {
    int *mem;                   // memory
    unsigned int ip;            // instruction pointer
    int w;                      // word register

    stack_t *ds;                // data stack
    stack_t *rs;                // return stack
    stack_t *fs;                // float stack

    hashtable_t *exe_tok;       // execution tokens

    state_t state;
    int base;

    int errno;
    char *err_msg;
} context_t;

#ifdef __cplusplus
}
#endif

#endif