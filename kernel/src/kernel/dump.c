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


int align(unsigned int addr, int line_size)
{
    return addr - (addr % line_size);
}

#define BYTES_PER_LINE 16
#define BYTES_PER_BLOCK 8

/**
 * Dump the contents of addr for size bytes into a list of strings.
 * It is the callers responsibility to free the returned pointer.
 */
char **dump(char *addr, int size, int columns)
{
    assert(size >= 0);
    assert(columns >= 0);

    const int bytes_per_line = columns * BYTES_PER_BLOCK;
    const int lines = (size / bytes_per_line) + 2;
    const int n = lines * 80;
    const char **ret = calloc(0, n + (sizeof(char*) * lines));
    if (ret == NULL)
        return NULL;

    char **ptr = ret;
    char *out = ret + lines + 2;

    const int extra_line = ((unsigned int)addr + size) % bytes_per_line == 0 ? 0 : bytes_per_line;
    const char *start = (char *)align((unsigned int)addr, bytes_per_line);
    const char *end = (char *)align((unsigned int)addr + size + extra_line, bytes_per_line);

    for (char *a = start; a < end; a += bytes_per_line)
    {
        *ptr++ = out;

        char buf[sizeof(unsigned int) + 1];
        itoa((unsigned int)a, buf, 16);

        out = rpad(out, buf, 8, '0');
        out = write(out, ':');
        out = write(out, ' ');
        out = write(out, ' ');

        for (int j = 0; j < columns; j++)
        {
            for (int i = 0; i < BYTES_PER_BLOCK; i++)
            {
                char *c = a + (j * BYTES_PER_BLOCK) + i;
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

        out = write(out, '|');
        for (int j = 0; j < columns; j++)
        {
            for (int i = 0; i < BYTES_PER_BLOCK; i++)
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

            if (j != columns - 1)
                out = write(out, ' ');
        }
        out = write(out, '|');

        out = write(out, '\0');
    }
    *ptr = 0;

    return ret;
}
