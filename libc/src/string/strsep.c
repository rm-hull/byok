#include <stdlib.h>
#include <string.h>

char *strsep(char **saveptr, const char *delim)
{
    char *str, *tmp;

    str = *saveptr;
    if (str == NULL)
        return NULL;

    tmp = strpbrk(str, delim);
    if (tmp)
    {
        *tmp = '\0';
        *saveptr = tmp + 1;
    }
    else
    {
        *saveptr = NULL;
    }

    return (char *)str;
}
