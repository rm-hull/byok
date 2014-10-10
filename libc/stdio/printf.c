#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__is_hulix_kernel)
#include <kernel/tty.h>
#endif

static void print(const char* data, size_t data_length)
{
    if ( data_length == 0 )
    {
        unsigned char c;
        while ((c = *data++) != '\0' )
            putchar(c);
    }
    else
    {
        for ( size_t i = 0; i < data_length; i++ )
            putchar((int) ((const unsigned char*) data)[i]);
    }
}

int printf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;
    size_t amount;
    bool rejected_bad_specifier = false;

    while ( *format != '\0' )
    {
        if ( *format != '%' )
        {
        print_c:
            amount = 1;
            while ( format[amount] && format[amount] != '%' )
                amount++;
            print(format, amount);
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format;

        if ( *(++format) == '%' )
            goto print_c;

        if ( rejected_bad_specifier )
        {
        incomprehensible_conversion:
            rejected_bad_specifier = true;
            format = format_begun_at;
            goto print_c;
        }

        if ( *format == 'c' )
        {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            print(&c, sizeof(c));
        }
        else if ( *format == 's' )
        {
            format++;
            const char* s = va_arg(parameters, const char*);
            print(s, 0);
        }
        else if ( *format == 'd' )
        {
            format++;
            const int i = va_arg(parameters, int);
            char *s = malloc(32);
            if (s == NULL) abort();
            itoa(i, s, 10);
            print(s, 0);
            free((void *)s);
        }
        else if ( *format == 'x' )
        {
            format++;
            const int i = va_arg(parameters, int);
            char *s = malloc(32);
            if (s == NULL) abort();
            itoa(i, s, 16);
            print(s, 0);
            free((void *)s);
        }
        else if ( *format == 'f' )
        {
            format++;
            const double d = va_arg(parameters, double);
            char *s = malloc(80);
            if (s == NULL) abort();
            dtoa(d, s);
            print(s, 0);
            free((void *)s);
        }
        else
        {
            goto incomprehensible_conversion;
        }
    }

    va_end(parameters);

    return written;
}
