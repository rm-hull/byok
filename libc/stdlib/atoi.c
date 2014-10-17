#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int atoi( char* str )
{
    int val = 0;
    char c, *p = str;
    while ((c = *str++) != NULL)
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
