#include <string.h>

char *strchr(const char *s, char value)
{
    char c, *ptr = (char *)s;
    while ((c = *ptr) != 0)
    {
        if (c == value) return ptr;
        ptr++;
    }

    return 0;
}
