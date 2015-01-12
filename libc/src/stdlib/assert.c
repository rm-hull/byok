#include <stdlib.h>
#include <stdio.h>

__attribute__((__noreturn__))
void __assert_failed(const char *expr, const char *file, int line, const char *func)
{
    printf("Assertion failed: '%s' in function %s(), at %s (line %d)\n", expr, func, file, line);
    while ( 1 ) { }
    __builtin_unreachable();
}
