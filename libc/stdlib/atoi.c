#include <stdlib.h>

int atoi( char* str )
{
    int signum = 1, val = 0;
    char c;

    if (*str == '-')
    {
        signum = -1;
        str++;
    }

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

    return val * signum;
}
