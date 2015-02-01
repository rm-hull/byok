#ifndef _COMMON_H
#define _COMMON_H 1

#include <collections/stack.h>

#include <stack_machine/context.h>

#define DELIMITERS " \t\n"

#define BUCKETS 256
#define READLINE_BUFSIZ 256
#define READLINE_HISTSIZ 100
#define MEMSIZ 16384

#define DEFAULT_BASE 10
#define DEFAULT_ECHO 0
#define CELL sizeof(int)

#define true 1
#define false 0

#define align(x) (((int)x + (CELL - 1)) & ~(CELL - 1))
extern int popnum(stack_t *stack, int *num);
extern int peeknum(stack_t *stack, int *num);
extern int pushnum(stack_t *stack, int num);
extern int printnum(int num, int base);
extern int parsenum(char *str, int *num, int base);
#endif
