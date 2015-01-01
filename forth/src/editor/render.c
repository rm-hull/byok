#include <stdio.h>
#include <stdlib.h>

#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/system.h>

#include <editor/colorize.h>
#include <editor/actions.h>
#include <editor/model.h>

#define SPACE " "
#define ROW_OFFSET 1
#define COL_OFFSET 6

void draw_frame(int block)
{
    terminal_clear();
    printf("Block #%d\n", block);
    for (int row = 0; row < ROWS; row++)
    {
        if (row < 10)
          printf(" ");

        printf("  %d\n", row);
    }

    printf("\n\n\nPress CTRL-X to exit");
}

void render_row(char *line, int row, context_t *ctx)
{
    position_t pos;
    pos.row = row + ROW_OFFSET;
    pos.column = COL_OFFSET;
    terminal_setcursor(&pos);

    char *buf = strdup(line);
    assert(buf != NULL);

    // Tokenize the line: this is so we can later colorize the words
    char *saveptr;
    char *token = strtok_r(buf, SPACE, &saveptr);

    // strtok_r() returns the first token: this means that if there is
    // preceeding whitespace then it is skipped over (not what we want)
    // so add some space to align properly
    for (int i = 0, n = token - buf; token != NULL &&  i < n; i++)
    {
        terminal_putchar(' ');
    }

    while (token != NULL)
    {
        // calculate the character position where the token appears in the line
        int index = token - buf;
        uint8_t color = colorize(token, line, index, ctx);
        terminal_setcolor(color);
        terminal_writestring(token);

        terminal_setcolor(COLOR_LIGHT_GREY);
        terminal_putchar(' ');

        token = strtok_r(NULL, SPACE, &saveptr);
    }

    // Draw spaces to blank out the remaining row
    terminal_getcursor(&pos);
    for (int i = pos.column; i < COLUMNS + COL_OFFSET; i++)
        terminal_putchar(' ');

    // Tidy up strdup allocation
    free(buf);
}

void render_model(editor_t *ed)
{
    // TODO: write message


    if (ed->render_op == MODEL_ERROR)
    {
        // TODO: Beep
        return;
    }

    if (ed->render_op == MODEL_REDRAW)
    {
        for (int row = ed->redraw_start_row; row <= ed->redraw_end_row; row++)
        {
            render_row(ed->data[row], row, ed->ctx);
        }
    }

    // Set cursor position
    position_t pos;
    pos.row = ROW_OFFSET + ed->row;
    pos.column = COL_OFFSET + ed->col;
    terminal_setcursor(&pos);

    if (ed->inputmode == INSERT)
        CURSOR_INSERT;
    else
        CURSOR_OVERWRITE;
}


/**
 * data should have 1024 bytes allocated
 */
void screen_editor(int block, char *data, context_t *ctx)
{
    // Save current screen contents
    screen_t save_to;
    save_to.buffer = malloc(VGA_BUFSIZ);
    terminal_save(&save_to);

    editor_t *ed = calloc(0, sizeof(editor_t));
    assert(ed != NULL);

    char *buf = strdup(data);
    assert(buf != NULL);

    hashtable_t *actions = actions_init();

    // Populate the data model
    char *saveptr;
    char *eachline = strtok_r(buf, "\n", &saveptr);
    for (int row = 0; row < ROWS; row++)
    {
        ed->data[row] = calloc(0, sizeof(char) * COLUMNS + 1);
        if (eachline != NULL)
        {
            int len = strlen(eachline);
            assert(len <= COLUMNS);
            memcpy(ed->data[row], eachline, len + 1);
            eachline = strtok_r(NULL, "\n", &saveptr);
        }
    }

    // initialize editor model
    ed->ctx = ctx;
    ed->inputmode = INSERT;
    ed->row = 0;
    ed->col = 0;
    model_redraw(ed, 0, ROWS - 1);

    // Initial rendering
    draw_frame(block);
    render_model(ed);

    // Edit loop
    while (ed->render_op != MODEL_EXIT)
    {
        ed->render_op = MODEL_NONE;
        ed->keycode = getchar();
        render_model(process_key(actions, ed));
    }

    // TODO: copy editor data lines into *data (join with \n)

    // Tidy up
    hashtable_destroy(actions);
    free(actions);

    terminal_restore(&save_to);
    free(save_to.buffer);
    free(buf);


    for (int row = 0; row < ROWS; row++)
        free(ed->data[row]);

    free(ed);
}
