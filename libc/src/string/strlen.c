#include <string.h>

int strlen(const char* str)
{
    if (str == NULL)
        return 0;

    int result = 0;
    while (str[result])
        result++;

    return result;
}
