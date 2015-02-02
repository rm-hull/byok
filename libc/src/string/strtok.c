#include <stdlib.h>
#include <string.h>

char *strtok_r(const char *str, const char *delim, char **saveptr)
{
    char *tmp;

    while (1)
    {
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

        if (*str != '\0')
        {
            return (char *)str;
        }
        else
        {
            str = NULL;
        }
    }
}

char *strtok(const char *str, const char *delim)
{
    static char *last = NULL;
    return strtok_r(str, delim, &last);
}
