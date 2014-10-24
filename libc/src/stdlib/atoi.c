#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* pos = "0123456789abcdefghijklmnopqrstuvwxyz";

int atoi( char* str, int base )
{
    assert(base >= 2 && base <= 36);

    int signum = 1, accum = 0, digit = 0;
    char c;

    if (*str == '-')
    {
        signum = -1;
        str++;
    }

    while ((c = *str++) != 0)
    {
        digit = strchr(pos, tolower(c)) - pos;
        if (digit >= 0 && digit < 36)
        {
            accum = (accum * base) + digit;
        }
        else
        {
            break;
        }
    }

    return accum * signum;
}
