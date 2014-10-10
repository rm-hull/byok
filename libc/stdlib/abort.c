#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int errno;

__attribute__((__noreturn__))
void __abort__(char *file, int line)
{
    // TODO: Add proper kernel panic.
    printf("Kernel Panic: abort() at %s (line %d)\n", file, line);
    while ( 1 ) { }
    __builtin_unreachable();
}
