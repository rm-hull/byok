#include <stdlib.h>
#include <stdio.h>

__attribute__((__noreturn__))
void __assert_failed(char *expr, char *file, int line)
{
    printf("Assertion failed: '%s' at %s (line %d)\n", expr, file, line);
    while ( 1 ) { }
    __builtin_unreachable();
}
