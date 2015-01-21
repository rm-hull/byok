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
    // NOTE: Can't use clear to EOL as count might be > VGA_WIDTH
    terminal_setcursor(posn);
    for (int i = 0; i < count; i++)
        terminal_putchar(' ');
}

/**
 * Find the start index of *buf for the token under the cursor index
 */
uint16_t rl_token_start(char *buf, uint16_t index)
{
    if (index == 0)
        return 0;

    assert(index > 0);

    // Go back until the pointer rests on a space
    // or is at the start of the buffer
    int start = index;
    while (start-- >= 0)
    {
        if (buf[start] == ' ' || start < 0)
        {
            start++;
            break;
        }
    }

    assert(start >= 0);
    assert(start <= index);
    return start;
}

uint16_t rl_token_end(char *buf, uint16_t index, uint16_t sz)
{
    assert(index < sz);

    char c;
    int end = max(0, index);
    while (end < sz)
    {
        c = buf[end];
        if (c == '\0' || c == ' ')
        {
            break;
        }
        end++;
    }

    assert(end < sz);
    assert(end >= index);
    return end;
}

int rl_replace_word(char *buf, char *orig_text, char *new_text, uint16_t index, uint16_t sz)
{
    int to_delete = strlen(orig_text);
    int to_insert = strlen(new_text);
    int start = index - to_delete;
    int end = rl_token_end(buf, index, sz);

    // bounds check
    // TODO: return status code rather than asserts
    //        or just set original text if new word wont fit
    assert(start + to_insert < sz);

    assert(start >= 0);
    assert(start <= index);

    assert(end < sz);
    assert(end >= index);

    // Delete the original text
    memmove(buf + start, buf + end, sz - end);

    // Now insert new text
    memmove(buf + start + to_insert, buf + start, sz - (start + to_insert));
    memmove(buf + start, new_text, to_insert);

    return to_insert - to_delete;
}

/**
 * Gets the token under the current cursor index.
 * Caller is responsible for freeing this string after use
 */
char *rl_get_token(char *buf, uint16_t index, uint16_t sz)
{
    int start = rl_token_start(buf, index);
    return strndup(buf + start, index - start);
}

void rl_advance_cursor(position_t *posn, int count)
{
    for (uint16_t i = 0; i < count; i++)
    {
        terminal_incrementcursor(posn);
    }
}

char *readline(char *buf, uint16_t sz, char **history, complete_t *completer, colorize_t *colorizer)
{
    // TODO: tab completion
    // TODO: parens matching
    // TODO: Dont clear buf on start (i.e. allow default value)
    // TODO: Handle scrolling better

    terminal_flush();
    keyboard_clear_buffer();
    memset(buf, 0, sz);

    uint16_t index = 0;
    uint16_t completion_state = 0;
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
            rl_advance_cursor(&cursor_posn, len - index);
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
        else if (c == KEY_TAB && completer != NULL)
        {
            char *orig_text = rl_get_token(buf, index, sz);
            char *new_text = completer->fn(orig_text, completion_state, completer->free_vars);
            if (new_text != NULL)
            {
                int n = rl_replace_word(buf, orig_text, new_text, index, sz);
                // TODO: Position cursor at end of word
                //rl_advance_cursor(&cursor_posn, n);
                //index += n;
                len = strlen(buf);
                completion_state++;
            }
            else
            {
                int n = rl_replace_word(buf, orig_text, orig_text, index, sz);
                len = strlen(buf);
                completion_state = 0;
            }
            free(orig_text);
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

        if (c != KEY_TAB)
        {
            completion_state = 0;
        }

        terminal_setcursor(&start_posn);
        terminal_colorstring(buf, colorizer);
        terminal_clear_eol();
        terminal_setcursor(&cursor_posn);
    }

    terminal_flush();
    return buf;
}
