#include <string.h>

char *strtoupper(char *s)
{
    for (int i = 0; s[i] != 0; i++)
        s[i] = toupper(s[i]);

    return s;
}
