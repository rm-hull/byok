#ifndef _INTERPRETER_H
#define _INTERPRETER_H 1

#include <stack_machine/context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern state_t interpret(context_t *ctx, char *in);

#ifdef __cplusplus
}
#endif

#endif