#ifndef _COMMON_H
#define _COMMON_H 1

#include <collections/stack.h>

#define true 1
#define false 0

extern int popnum(stack_t *stack, int *num);
extern int peeknum(stack_t *stack, int *num);
extern int pushnum(stack_t *stack, int num);
extern int printnum(int num, int base);
extern int parsenum(char *str, int *num, int base);
#endif
