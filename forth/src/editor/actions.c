#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/kb.h>

#include <editor/model.h>
#include <editor/actions.h>
#include <collections/hashtable.h>

int is_empty(char *text)
{
    int count = 0;
    char c;
    while ((c = *text++) != '\0')
        if (!isspace(c)) count++;

    return count == 0;
}

editor_t *paste(editor_t *ed, int buffer)
{
    char *currline = ed->data[ed->row];
    int len = strlen(currline);
    int yank_len = strlen(ed->yank[buffer]);

    if (len + yank_len < COLUMNS)
    {
        char *dest = currline + ed->col;
        int len = strlen(dest);

        // Make space for the insertion
        memmove(dest + yank_len, dest, len + 1);

        // Graft in the content of the yank buffer, and advance the column position
        memcpy(dest, ed->yank[buffer], yank_len);
        ed->col += yank_len;
        return model_redraw(ed, ed->row, ed->row);
    }

    return model_error(ed, NULL);
}

editor_t *default_handler(editor_t *ed)
{
    if (!isprint(ed->input.keycode) || ed->input.flags.control || ed->input.flags.alt)
    {
        return model_error(ed, NULL);
    }

    char *currline = ed->data[ed->row];
    if (ed->inputmode == INSERT)
    {
        char *src = currline + ed->col;
        int len = strlen(src);

        // If there is text after the current cursor position then
        // right-trim it so that this character can be inserted, and
        // recalc the length.
        if (len > 0)
        {
            src = rtrim(src);
            len = strlen(src);
        }

        // Check to see if an extra character can be inserted
        if (ed->col + len < COLUMNS)
        {
            // Shift contents of currline (at cursor pos) one place to right
            // including the terminating zero
            char *dest = src + 1;
            memmove(dest, src, len + 1);
        }
        else
        {
            // Not enough space at EOL to insert
            return model_error(ed, NULL);
        }
    }

    if (ed->col < COLUMNS)
    {
        currline[ed->col] = ed->input.keycode;
        ed->col++;
        return model_redraw(ed, ed->row, ed->row);
    }
    else
    {
        return model_error(ed, NULL);
    }
}

editor_t *key_up_handler(editor_t *ed)
{
    if (ed->row == 0)
    {
        // Can't go up any further
        return model_error(ed, NULL);
    }

    ed->row--;

    // Now check if current col position is beyond end of the current line
    char *currline = ed->data[ed->row];
    ed->col = min(ed->col, strlen(currline));

    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *key_down_handler(editor_t *ed)
{
    if (ed->row == ROWS - 1)
    {
        // Can't go down any further
        return model_error(ed, NULL);
    }

    ed->row++;

    // Now check if current col position is beyond end of the current line
    char *currline = ed->data[ed->row];
    ed->col = min(ed->col, strlen(currline));

    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *key_left_handler(editor_t *ed)
{
    if (ed->col > 0)
    {
        ed->col--;
    }
    else if (ed->row > 0)
    {
        ed->row--;
        char *currline = ed->data[ed->row];
        ed->col = strlen(currline);
    }

    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *key_right_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    if (ed->col < strlen(currline))
    {
        ed->col++;
    }
    else if (ed->row < ROWS - 1)
    {
        ed->row++;
        ed->col = 0;
    }

    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *key_insert_handler(editor_t *ed)
{
    ed->inputmode = ed->inputmode == INSERT ? OVERWRITE : INSERT;
    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *delete_char_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    int len = strlen(currline);
    char *dest = currline + ed->col;
    char *src = dest + 1;

    if (ed->col < len)
    {
        // Pull contents of currline (at cursor) in one place
        // including the terminating zero
        memmove(dest, src, strlen(src) + 1);
        return model_redraw(ed, ed->row, ed->row);
    }
    else if (ed->row < ROWS - 1)
    {
        // At the end of the line, so lets try and pull up the next
        // line onto this line (space permitting)
        char *nextline = ed->data[ed->row + 1];
        int next_len = strlen(nextline);
        if (next_len + len <= COLUMNS)
        {
            // stitch the next line onto the end of the current line
            memmove(dest, nextline, next_len + 1);

            // Shuffle all subsequent rows up one slot
            for (int row = ed->row + 1; row < ROWS - 1; row++)
                memmove(ed->data[row], ed->data[row + 1], COLUMNS + 1);

            // And clear out the last row altogether
            memset(ed->data[ROWS - 1], 0, COLUMNS + 1);
            return model_redraw(ed, ed->row, ROWS - 1);
        }
    }

    // Was either on the last line, or there is no space to
    // pull the next line onto the current line
    return model_error(ed, NULL);
}

editor_t *delete_to_EOL_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    char *src = currline + ed->col;
    int len = strlen(src);

    if (ed->col < strlen(currline))
    {
        rl_yank(ed->yank, src, len, COLUMNS + 1);

        // Delete to EOL
        memset(src, 0, COLUMNS - ed->col);
        return model_redraw(ed, ed->row, ed->row);
    }
    return model_error(ed, NULL);
}

editor_t *delete_to_cursor_handler(editor_t *ed)
{
    if (ed->col > 0)
    {
        char *currline = ed->data[ed->row];
        char *src = currline + ed->col;
        char *dest = currline;
        int len = strlen(src);

        rl_yank(ed->yank, dest, ed->col, COLUMNS + 1);

        // Delete to cursor
        memmove(dest, src, len + 1);
        ed->col = 0;
        return model_redraw(ed, ed->row, ed->row);
    }
    return model_error(ed, NULL);
}

editor_t *delete_word_prev_boundary_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    int start = rl_token_start(currline, rl_prev_word(currline, ed->col));
    int yank_len = ed->col - start;

    if (yank_len > 0)
    {
        char *src = currline + ed->col;
        char *dest = currline + start;

        rl_yank(ed->yank, dest, yank_len, COLUMNS + 1);

        // Delete word ⇦ until after the previous word boundary
        memmove(dest, src, COLUMNS + 1 - ed->col);
        ed->col -= yank_len;
        return model_redraw(ed, ed->row, ed->row);
    }
    return model_error(ed, NULL);
}

editor_t *delete_word_next_boundary_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    int end = rl_token_end(currline, rl_next_word(currline, ed->col), COLUMNS + 1);
    int yank_len = end - ed->col;

    if (yank_len > 0)
    {
        char *src = currline + end;
        char *dest = currline + ed->col;

        rl_yank(ed->yank, dest, yank_len, COLUMNS + 1);

        // Delete word ⇨ until before the next word boundary
        memmove(dest, src, COLUMNS + 1 - ed->col);
        return model_redraw(ed, ed->row, ed->row);
    }
    return model_error(ed, NULL);
}

editor_t *key_backspace_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    char *src = currline + ed->col;
    int len = strlen(src);

    if (ed->col > 0)
    {
        // Shift contents of currline (at cursor) one place to left
        // including the terminating zero
        char *dest = src - 1;
        memmove(dest, src, len + 1);

        ed->col--;
        return model_redraw(ed, ed->row, ed->row);
    }
    else if (ed->row > 0)
    {
        // Check to see if there is enough space to pull the current
        // line onto the previous line
        char *prevline = ed->data[ed->row - 1];
        int prev_len = strlen(prevline);
        if (prev_len + len <= COLUMNS)
        {
            // stitch the current line onto the end of the previous line
            memmove(prevline + prev_len, currline, len + 1);

            // Shuffle all subsequent rows up one slot
            for (int row = ed->row; row < ROWS - 1; row++)
                memmove(ed->data[row], ed->data[row + 1], COLUMNS + 1);

            // And clear out the last row altogether
            memset(ed->data[ROWS - 1], 0, COLUMNS + 1);

            // Finally, set the cursor position at the end of what was
            // the previous line
            ed->col = prev_len;
            ed->row--;
            return model_redraw(ed, ed->row, ROWS - 1);
        }
    }

    // Got here? prob'ly because there was not enough
    // space to concat prev + curr lines, or was on the
    // first line
    return model_error(ed, NULL);
}

#define swap(a, b) { char _tmp = a; a = b; b = _tmp; }

editor_t *transpose_char_handler(editor_t *ed)
{
    if (ed->col > 0)
    {
        char *currline = ed->data[ed->row];
        int len = strlen(currline);
        int offset = ed->col == len ? ed->col - 1 : ed->col;
        swap(currline[offset - 1], currline[offset]);
        if (ed->col < len)
        {
            ed->col++;
        }
        return model_redraw(ed, ed->row, ed->row);
    }

    return model_error(ed, NULL);
}

editor_t *paste_prev_killed_text_handler(editor_t *ed)
{
    return paste(ed, 0);
}

editor_t *paste_prev_prev_killed_text_handler(editor_t *ed)
{
    return paste(ed, 1);
}

editor_t *key_tab_handler(editor_t *ed)
{
    // Calculate the number of spaces required to the next tabstop
    // from the current cursor position.
    int n = TABSTOPS - (ed->col % TABSTOPS);

    // Alter the keycode, to replace tab with spaces, and then
    // repeatedly call the default handler to insert/overwrite
    // spaces.
    ed->input.keycode = ' ';
    for (int i = 0; i < n; i++)
    {
        ed = default_handler(ed);
    }

    return ed;
}

editor_t *key_newline_handler(editor_t *ed)
{
    // check to see if there is nothing on the last row:
    // if so, everything can be moved down line, else error
    if (is_empty(ed->data[ROWS - 1]))
    {
        // Shuffle all the lines down one position
        char *currline = ed->data[ed->row];
        for (int row = ROWS - 2; row > ed->row; row--)
            memmove(ed->data[row + 1], ed->data[row], COLUMNS + 1);

        // Split the line at the current cursor position, moving any
        // text after the cursor onto the next line
        char *src = currline + ed->col;
        char *dest = ed->data[ed->row + 1];
        int len = strlen(src);
        memset(dest, 0, COLUMNS + 1);
        memmove(dest, src, len + 1);

        // And clear out the rest of the current line at the current position
        memset(src, 0, COLUMNS + 1 - len);

        // Finally, set the cursor position on the next line
        ed->row++;
        ed->col = 0;
        return model_redraw(ed, ed->row - 1, ROWS - 1);
    }

    return model_error(ed, NULL);
}

editor_t *key_home_handler(editor_t *ed)
{
    ed->col = 0;
    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *key_end_handler(editor_t *ed)
{
    char *currline = ed->data[ed->row];
    ed->col = strlen(currline);
    ed->render_op = MODEL_NONE;
    return ed;
}

editor_t *exit_handler(editor_t *ed)
{
    ed->render_op = MODEL_EXIT;
    return ed;
}

#define ctrl_handler(name, h) \
    editor_t *key_##name##_handler(editor_t *ed) \
    { return ed->input.flags.control ? h(ed) : default_handler(ed); }

#define alt_handler(name, h) \
    editor_t *key_##name##_handler(editor_t *ed) \
    { return ed->input.flags.alt ? h(ed) : default_handler(ed); }

ctrl_handler(ctrl_a, key_home_handler);
ctrl_handler(ctrl_d, delete_char_handler);
ctrl_handler(ctrl_e, key_end_handler);
ctrl_handler(ctrl_k, delete_to_EOL_handler);
ctrl_handler(ctrl_t, transpose_char_handler);
ctrl_handler(ctrl_u, delete_to_cursor_handler);
ctrl_handler(ctrl_w, delete_word_prev_boundary_handler);
ctrl_handler(ctrl_x, exit_handler);

editor_t *key_alt_ctrl_d_handler(editor_t *ed) \
{
    if (ed->input.flags.alt) return delete_word_next_boundary_handler(ed);
    if (ed->input.flags.control) return delete_char_handler(ed);
    return default_handler(ed);
}

editor_t *key_alt_ctrl_y_handler(editor_t *ed) \
{
    if (ed->input.flags.alt) return paste_prev_prev_killed_text_handler(ed);
    if (ed->input.flags.control) return paste_prev_killed_text_handler(ed);
    return default_handler(ed);
}


int add_action(hashtable_t *htbl, unsigned char scancode, editor_t *(*fn)(editor_t *))
{
    assert(htbl != NULL);
    assert(fn != NULL);

    action_t *action = malloc(sizeof(action_t));
    if (action == NULL)
        return -1;

    action->scancode = scancode;

    // dont care what the return status is:
    // if it existed it was deleted, if it didnt, fine: nothing to do
    hashtable_remove(htbl, (void **)&action);

    action->fn = fn;
    return hashtable_insert(htbl, action);
}

int find_action(hashtable_t *htbl, char scancode, action_t **action)
{
    assert(htbl != NULL);

    action_t *data = malloc(sizeof(action_t));
    if (data == NULL)
        return -1;

    data->scancode = scancode;
    action_t *tmp = data;   // copy data ptr, so it can be freed later
    int retval;
    if ((retval = hashtable_lookup(htbl, (void **)&data)) == 0)
    {
        *action = data;
    }

    free(tmp);

    return retval;
}

int action_hash(const void *data)
{
    action_t *action = (action_t *)data;
    return abs(action->scancode * 41);
}

int action_match(const void *data1, const void *data2)
{
    action_t *action1 = (action_t *)data1;
    action_t *action2 = (action_t *)data2;

    return action1->scancode == action2->scancode;
}

editor_t *process_key(hashtable_t *htbl, editor_t *ed)
{
    assert(htbl != NULL);
    assert(ed != NULL);

    action_t *action;
    if (find_action(htbl, ed->input.scancode, &action) == 0)
    {
        return action->fn(ed);
    }
    else
    {
        return default_handler(ed);
    }
}

hashtable_t *actions_init()
{
    hashtable_t *htbl = malloc(sizeof(hashtable_t));
    assert(htbl != NULL);

    hashtable_init(htbl, 20 /*BUCKETS*/, action_hash, action_match, free);

    add_action(htbl, SCANCODE_UP, key_up_handler);
    add_action(htbl, SCANCODE_DOWN, key_down_handler);
    add_action(htbl, SCANCODE_LEFT, key_left_handler);
    add_action(htbl, SCANCODE_RIGHT, key_right_handler);
    add_action(htbl, SCANCODE_INSERT, key_insert_handler);
    add_action(htbl, SCANCODE_DELETE, delete_char_handler);
    add_action(htbl, SCANCODE_BACKSPACE, key_backspace_handler);
    add_action(htbl, SCANCODE_TAB, key_tab_handler);
    add_action(htbl, SCANCODE_ENTER, key_newline_handler);
    add_action(htbl, SCANCODE_HOME, key_home_handler);
    add_action(htbl, SCANCODE_END, key_end_handler);
    add_action(htbl, SCANCODE_A, key_ctrl_a_handler);
    add_action(htbl, SCANCODE_D, key_alt_ctrl_d_handler);
    add_action(htbl, SCANCODE_E, key_ctrl_e_handler);
    add_action(htbl, SCANCODE_K, key_ctrl_k_handler);
    add_action(htbl, SCANCODE_T, key_ctrl_t_handler);
    add_action(htbl, SCANCODE_U, key_ctrl_u_handler);
    add_action(htbl, SCANCODE_W, key_ctrl_w_handler);
    add_action(htbl, SCANCODE_X, key_ctrl_x_handler);
    add_action(htbl, SCANCODE_Y, key_alt_ctrl_y_handler);

    // TODO insert/overwrite toggle

    return htbl;
}
