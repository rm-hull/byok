#include <stdlib.h>
#include <stdio.h>

/*
 * re_fail:
 *  default internal error handler for re_exec.
 *
 *  should probably do something like a longjump to recover
 *  gracefully.
 */
void
re_fail(char *s, char c)
{
    printf("re_fail: %s [opcode 0x%x]\n", s, c);
    abort();
}
