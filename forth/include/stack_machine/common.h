#ifndef _COMMON_H
#define _COMMON_H 1

#include <collections/stack.h>

#include <stack_machine/context.h>

#define BASE "BASE"
#define DEFAULT_BASE 10

#define true 1
#define false 0

extern int popnum(stack_t *stack, int *num);
extern int peeknum(stack_t *stack, int *num);
extern int pushnum(stack_t *stack, int num);
extern word_t get_variable(context_t *ctx, char *name, int default_value);
extern int printnum(int num, int base);
extern int parsenum(char *str, int *num, int base);
#endif
