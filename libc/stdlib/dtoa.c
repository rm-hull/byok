#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PRECISION 0.00000000000001
#define MAX_NUMBER_STRING_SIZE 32

/* Double to ASCII - from http://stackoverflow.com/a/7097567/260541 */
char * dtoa( double n, char *s )
{
    // handle special cases
    if (isnan(n))
    {
        memcpy(s, "nan", 4);
    }
    else if (isinf(n))
    {
        memcpy(s, "inf", 4);
    }
    else if (n == 0.0)
    {
        memcpy(s, "0", 2);
    }
    else
    {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
        // calculate magnitude
        m = log10(n);
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        // set up for scientific notation
        if (useExp)
        {
            if (m < 0)
               m -= 1.0;
            n = n / pow(10.0, m);
            m1 = m;
            m = 0;
        }
        if (m < 1.0)
        {
            m = 0;
        }
        // convert the number
        while (n > PRECISION || m >= 0)
        {
            double weight = pow(10.0, m);
            if (weight > 0 && !isinf(weight))
            {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (m == 0 && n > 0)
                *(c++) = '.';
            m--;
        }
        if (useExp)
        {
            // convert the exponent
            int i, j;
            *(c++) = 'e';
            if (m1 > 0)
            {
                *(c++) = '+';
            }
            else
            {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0)
            {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--)
            {
                // swap without temporary
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
    }
    return s;
}
