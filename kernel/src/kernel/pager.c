#include <stdlib.h>
#include <stdio.h>

#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/system.h>

void show_footer(int line, int max)
{
    position_t bottom_left;
    bottom_left.row = VGA_HEIGHT - 1;
    bottom_left.column = 0;

    terminal_setcursor(&bottom_left);
    terminal_setcolor(0x80);
    printf(" 'q' to exit. line: %d/%d (%d%%) ", line + 1, max, (line + 1) * 100 / max);
    terminal_setcolor(0x07);
    terminal_writestring("   ");
    terminal_setcursor(&bottom_left);
}

int count_lines(char **text)
{
    int count = 0;
    while (*text++ != NULL)
        count++;

    return count;
}

int pager(char **text)
{
    CURSOR_HIDE;

    screen_t save_to;
    position_t top_left;

    top_left.row = 0;
    top_left.column = 0;

    save_to.buffer = malloc(VGA_BUFSIZ);
    if (save_to.buffer == NULL)
        return -1;
    terminal_save(&save_to);

    int doclen = count_lines(text);
    int x_offset = 0;
    int y_offset = 0;

    while (true)
    {
        terminal_setcursor(&top_left);
        terminal_setcolor(0x07);

        bool eol = false;
        bool eof = false;

        for (int j = 0; j < VGA_HEIGHT - 1; j++)
        {
            char *line = text[y_offset + j];
            if (eof || line == NULL)
            {
                eol = eof = true;
            }
            else
            {
                eol = false;
            }

            for (int i = 0; i < VGA_WIDTH; i++)
            {
                char c = line[x_offset + i];
                if (!eol && c == '\0') eol = true;

                terminal_putchar(eol ? ' ' : c);
            }
        }

        show_footer(y_offset, doclen);

        char keypress = tolower(getchar());
        if (keypress == 'q')
        {
            break;
        }
        else if (keypress == -56 /*KEY_UP*/)
        {
            y_offset--;
        }
        else if (keypress == -57 /*KEY_PGUP*/)
        {
            y_offset -= 10;
        }
        else if (keypress == -50 /*KEY_DOWN*/)
        {
            y_offset++;
        }
        else if (keypress == -51 /*KEY_PGDN*/)
        {
            y_offset += 10;
        }

        if (y_offset < 0)
        {
            y_offset = 0;
        }
        else if (y_offset > doclen - 1)
        {
            y_offset = doclen -1;
        }
    }

    terminal_restore(&save_to);
    free(save_to.buffer);

    return 0;
}
