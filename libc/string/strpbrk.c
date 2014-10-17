#include <stdlib.h>
#include <string.h>

char *strpbrk(const char *str, const char *set)
{
    const char *x;
    for (; *str; str++)
        for (x = set; *x; x++)
            if (*str == *x)
                return (char *)str;

    return NULL;
}
