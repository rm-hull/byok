#include <stdlib.h>
#include <kernel/system.h>

char *sbrk (unsigned amt)
{
    extern char _heap;
    static char *ptr = 0;
    char *res;

    if (ptr == 0)
        ptr = &_heap;

    if (amt == 0)
        return (char *)ptr;

    if (((long)ptr) % 8)
        ptr = ptr + (8 - (((long)(ptr)) % 8));

    res = ptr;
    ptr += amt;
    return (char *)res;
}
