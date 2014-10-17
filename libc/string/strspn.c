#include <stdlib.h>
#include <string.h>

char *strspn(const char *s1, const char *s2)
{
    const char *p = s1, *spanp;
    char c, sc;
cont:
    c = *p++;
    for (spanp = s2; (sc = *spanp++) != 0;)
        if (sc == c)
            goto cont;

    return (char *)(p - 1 - s1);
}
