#include <string.h>

int strlen(const char* string)
{
    int result = 0;
    while ( string[result] )
        result++;
    return result;
}
