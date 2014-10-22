#include <stdlib.h>
#include <string.h>

char *strtok(const char *str, const char *delim)
{
    static char *last = NULL;
    char *tmp;

    if (str == NULL)
    {
        str = last;
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
        last = tmp + 1;
    }
    else
    {
        last = NULL;
    }

    return (char *)str;
}
