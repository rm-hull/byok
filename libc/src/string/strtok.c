#include <stdlib.h>
#include <string.h>

char *strtok_r(const char *str, const char *delim, char **saveptr)
{
    char *tmp;

    if (str == NULL)
    {
        str = *saveptr;
        if (str == NULL)
            return NULL;
    }
    else
    {
        str += (int)strspn(str, delim);
    }

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

char *strtok(const char *str, const char *delim)
{
    static char *last = NULL;
    return strtok_r(str, delim, &last);
}
