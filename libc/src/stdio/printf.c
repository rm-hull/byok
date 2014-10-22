#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/tty.h>

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

int __formatter(void (*consume)(const char *, size_t), const char* restrict format, va_list parameters)
{
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
            consume(format, amount);
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
            consume(&c, sizeof(c));
        }
        else if ( *format == 's' )
        {
            format++;
            const char* s = va_arg(parameters, const char*);
            consume(s, 0);
        }
        else if ( *format == 'd' )
        {
            format++;
            const int i = va_arg(parameters, int);
            char *s = malloc(32);
            if (s == NULL) abort();
            itoa(i, s, 10);
            consume(s, 0);
            free((void *)s);
        }
        else if ( *format == 'x' )
        {
            format++;
            const int i = va_arg(parameters, int);
            char *s = malloc(32);
            if (s == NULL) abort();
            itoa(i, s, 16);
            consume(s, 0);
            free((void *)s);
        }
        else if ( *format == 'f' )
        {
            format++;
            const double d = va_arg(parameters, double);
            char *s = malloc(80);
            if (s == NULL) abort();
            dtoa(d, s);
            consume(s, 0);
            free((void *)s);
        }
        else
        {
            goto incomprehensible_conversion;
        }
    }

    return written;
}

int vprintf(const char* restrict format, va_list parameters)
{
    return __formatter(&print, format, parameters);
}

int printf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);
    int retval = vprintf(format, parameters);
    va_end(parameters);
    return retval;
}
