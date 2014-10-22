#include <stdlib.h>
#include <string.h>

char *strdup(const char* str)
{
    char *p;
    int len = strlen(str) + 1;
    if ((p = malloc(len)) == NULL)
        return NULL;

    memcpy(p, str, len);
    return p;
}
