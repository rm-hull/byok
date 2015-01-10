#include <stdlib.h>
#include <string.h>

// Copy at most n bytes of str
char *strndup(const char* str, int n)
{
    char *p;
    int len = strlen(str);

    if (len > n)
        len = n;

    if ((p = malloc(len + 1)) == NULL)
        return NULL;

    memcpy(p, str, len);
    p[len] = '\0';
    return p;
}
