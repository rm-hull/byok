#ifndef _COMPILER_H
#define _COMPILER_H 1

#include <stack_machine/context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void literal(context_t *ctx, int n);
extern void compile(context_t *ctx, int n, ...);

#ifdef __cplusplus
}
#endif

#endif