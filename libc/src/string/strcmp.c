#include <string.h>

int strcmp(const char *aptr, const char *bptr)
{
    char c1, c2;
    int v;

    do {
        c1 = *aptr++;
        c2 = *bptr++;
        v = (unsigned int)c1 - (unsigned int)c2;
    } while ((v == 0) && (c1 != '\0'));

    if (v == 0)
        return 0;
    else if (v < 0)
        return -1;
    else
        return +1;
}
