#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// http://stackoverflow.com/a/1431206/260541
char *ltrim(char *s)
{
    if (s == NULL) return NULL;
    char *ptr = s;
    while(isspace(*ptr)) ptr++;
    if (s != ptr) memmove(s, ptr, strlen(ptr)+1);
    return s;
}

char *rtrim(char *s)
{
    if (s == NULL) return NULL;
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}
