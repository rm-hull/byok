#include <stdio.h>
#include <stdlib.h>
#include <kernel/system.h>
#include <kernel/tty.h>
#include <kernel/kb.h>
#include <kernel/vga.h>


/* inserts the given character 'c' in 's' at the specified index, but
   only if there is enough space (according to sz). Returns true if
   successfully updated, else false if full. */
unsigned int rl_insert(char *s, uint16_t index, char c, uint16_t sz)
{
    assert(index < sz);

    int len = strlen(s + index);
    if (index + len < sz - 1)  // still got one character of space we can insert into ?
    {
        // Shift the contents from index right one character
        memmove(s + index + 1, s + index, len);
        s[index] = c;
        return true;
    }
    else
    {
        // Dont insert anything and just return
        return false;
    }
}

/* removes the character at index - 1 in 's': used to implement 'backspace'
   style deletion where characters are consumed from the right. Returns
   true if characters were deleted, false otherwise (which occurs when
   trying to remove character at position zero. */
unsigned int rl_remove(char *s, uint16_t index, uint16_t sz)
{
    assert(index < sz);

    if (index == 0)
    {
        return false;
    }
    else
    {
        char *dest = s + index - 1;
        char *src = s + index;
        memmove(dest, src, strlen(src) + 1);
        return true;
    }
}

/* removes the character at index in 's': used to implement 'delete' style
   deletion where characters are consumed from the left. */
uint16_t rl_delete(char *s, uint16_t index, uint16_t sz)
{
    assert(index < sz);

    char *dest = s + index;
    char *src = s + index + 1;
    memmove(dest, src, strlen(src) + 1);
    return true;
}

uint16_t rl_count_hist(char **history)
{
    uint16_t count = 0;
    while (*history++ != NULL)
        count++;

    return count;
}

void rl_clear(position_t* posn, uint16_t count)
{
    terminal_setcursor(posn);
    for (int i = 0; i < count; i++)
        terminal_putchar(' ');
}

char *readline(char *buf, uint16_t sz, char **history, colorize_t *colorizer)
{
    // TODO: tab completion
    // TODO: parens matching
    // TODO: Dont clear buf on start (i.e. allow default value)
    // TODO: Handle scrolling better


    terminal_flush();
    keyboard_clear_buffer();
    memset(buf, 0, sz);

    uint16_t index = 0;
    uint16_t len = strlen(buf);
    position_t start_posn;
    position_t cursor_posn;

    int hist_size = rl_count_hist(history);
    int hist_index = hist_size > 0 ? hist_size : 0;

    terminal_getcursor(&start_posn);
    terminal_getcursor(&cursor_posn);

    while (1)
    {
        CURSOR_INSERT;
        char c = getchar();
        CURSOR_HIDE;

        if (c == KEY_LEFT && index > 0)
        {
            index--;
            terminal_decrementcursor(&cursor_posn);
            terminal_setcursor(&cursor_posn);
            continue;
        }
        else if (c == KEY_RIGHT && index < len)
        {
            index++;
            terminal_incrementcursor(&cursor_posn);
            terminal_setcursor(&cursor_posn);
            continue;
        }
        else if ((c == KEY_UP || c == KEY_DOWN) && hist_size > 0)
        {
            hist_index += (c == KEY_UP) ? -1 : 1;
            if (hist_index < 0)
                hist_index = hist_size -1;
            else if (hist_index >= hist_size)
                hist_index = 0;

            rl_clear(&start_posn, len);
            char *src = history[hist_index];
            len = strlen(src);
            memset(buf, 0, sz);
            memcpy(buf, src, len + 1);

            while (index > len)
            {
                index--;
                terminal_decrementcursor(&cursor_posn);
            }
        }
        else if (c == KEY_CTRL_A || c == KEY_HOME)
        {
            for (uint16_t i = 0; i < index; i++)
            {
                terminal_decrementcursor(&cursor_posn);
            }
            index = 0;
        }
        else if (c == KEY_CTRL_E || c == KEY_END)
        {
            for (uint16_t i = index; i < len; i++)
            {
                terminal_incrementcursor(&cursor_posn);
            }
            index = len;
        }
        else if (c == KEY_DELETE)
        {
            if (index < sz && rl_delete(buf, index, sz))
            {
                len--;
            }
        }
        else if (c == KEY_BACKSPACE)
        {
            if (index > 0 && rl_remove(buf, index, sz))
            {
                index--;
                len--;
                terminal_decrementcursor(&cursor_posn);
            }
        }
        else if (c == KEY_NEW_LINE)
        {
            terminal_putchar('\n');
            break;
        }
        else if (isprint(c) && index < sz - 1 && rl_insert(buf, index, c, sz))
        {
            index++;
            len++;
            terminal_incrementcursor(&cursor_posn);
        }
        else if (c < 0) {
            //printf("     extended=%d  ", c);
        }


        terminal_setcursor(&start_posn);
        terminal_colorstring(buf, colorizer);
        terminal_putchar(' ');
        terminal_setcursor(&cursor_posn);
    }

    terminal_flush();
    return buf;
}
