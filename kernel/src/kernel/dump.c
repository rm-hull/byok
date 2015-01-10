#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char *write(char *out, char c)
{
    *out++ = c;
    return out;
}

char *rpad(char *out, char *in, int size, char pad_char)
{
    int n = strlen(in);

    for (int i = 0; i < size - n; i++)
    {
        out = write(out, pad_char);
    }

    memcpy(out, in, n);
    return out + n;
}

char *safe_puts(char *out, char *in, int size)
{
    for (int i = 0; i < size; i++)
    {
        char c = in[i];
        out = write(out, isprint(c) ? c : '.');
    }
    return out;
}

char *hex_bytes(char *out, char *in, int size)
{
    char buf[3];

    for (int i = 0; i < size; i++)
    {
        unsigned char value = in[i];
        itoa(value, buf, 16);
        out = rpad(out, buf, 2, '0');
        out = write(out, ' ');
    }

    return out;
}


int align(unsigned int addr)
{
    return addr - (addr % 16);
}

/**
 * Dump the contents of addr for size bytes into a list of strings.
 * It is the callers responsibility to free the returned pointer.
 */
char **dump(char *addr, int size)
{
    int lines = (size / 16) + 2;
    int n = lines * 80;
    char **ret = calloc(0, n + (sizeof(char*) * lines));
    if (ret == NULL)
    {
        return NULL;
    }

    char **ptr = ret;
    char *out = ret + lines + 2;

    int extra_line = ((unsigned int)addr + size) % 16 == 0 ? 0 : 16;
    char *start = (char *)align((unsigned int)addr);
    char *end = (char *)align((unsigned int)addr + size + extra_line);

    for (char *a = start; a < end; a += 16)
    {
        *ptr++ = out;

        char buf[sizeof(unsigned int) + 1];
        itoa((unsigned int)a, buf, 16);

        out = rpad(out, buf, 8, '0');
        out = write(out, ':');
        out = write(out, ' ');
        out = write(out, ' ');

        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                char *c = a + (j * 8) + i;
                if (c >= addr && c < addr + size)
                {
                    out = hex_bytes(out, c, 1);
                }
                else
                {
                    out = write(out, ' ');
                    out = write(out, ' ');
                    out = write(out, ' ');
                }
            }
            out = write(out, ' ');
        }

        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                char *c = a + (j * 8) + i;
                if (c >= addr && c < addr + size)
                {
                    out = safe_puts(out, c, 1);
                }
                else
                {
                    out = write(out, ' ');
                }
            }
            out = write(out, ' ');
        }

        out = write(out, '\0');
    }
    *ptr = 0;

    return ret;
}
