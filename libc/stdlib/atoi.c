#include <stdlib.h>

int atoi( char* str )
{
    int val = 0;
    char c;
    while ((c = *str++) != 0)
    {
        if (c >= '0' && c <= '9')
        {
            val = (val * 10) + (c - '0');
        }
        else
        {
            break;
        }
    }

    return val;
}
