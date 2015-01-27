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
          terminal_putchar(' ');

        printf("  %d\n", row);
    }

    printf("\n\n\nPress CTRL-X to exit");
}

void draw_pos(int row, int column)
{
    position_t pos = { .row = ROWS + 4, .column = COLUMNS - 2 };
    terminal_setcursor(&pos);

    printf("(%d,%d)", column, row);
    terminal_clear_eol();
}

void render_row(char *line, int row, context_t *ctx)
{
    position_t pos = { .row = row + ROW_OFFSET, .column = COL_OFFSET };
    terminal_setcursor(&pos);

    colorize_t colorizer = { .fn = &colorize, .free_vars = ctx };
    terminal_colorstring(line, &colorizer);

    // Draw spaces to blank out the remaining row
    terminal_getcursor(&pos);
    for (int i = pos.column; i < COLUMNS + COL_OFFSET; i++)
        terminal_putchar(' ');
}

void render_model(editor_t *ed)
{
    draw_pos(ed->row, ed->col);

    // TODO: write message

    if (ed->render_op == MODEL_ERROR)
    {
        // TODO: Beep
    }

    if (ed->render_op == MODEL_REDRAW)
        for (int row = ed->redraw_start_row; row <= ed->redraw_end_row; row++)
            render_row(ed->data[row], row, ed->ctx);

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

char *write_rows(char *dest, editor_t *ed)
{
    memset(dest, 0, ROWS * COLUMNS);

    char c, *line;
    int i = 0;
    for (int row = 0; row < ROWS; row++)
    {
        line = ed->data[row];

        // always write a newline regardless of whether there is any data
        // on the current line. When this comes to be re-edited, we want
        // the same vertical spacing retained
        while ((c = *line++) != '\0')
            dest[i++] = c;

        dest[i++] = '\n';
    }

    return dest;
}

editor_t *create_model(context_t *ctx, char *data)
{
    // Caller is responsible for calling destroy_model()
    editor_t *ed = calloc(0, sizeof(editor_t));
    assert(ed != NULL);

    // initialize editor model
    ed->ctx = ctx;
    ed->inputmode = INSERT;
    ed->row = 0;
    ed->col = 0;
    ed->yank = calloc(0, sizeof(char) * COLUMNS + 1);
    model_redraw(ed, 0, ROWS - 1);

    char *buf = strdup(data);
    assert(buf != NULL);

    // Populate the editor model from the buffer
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
    free(buf);

    return ed;
}

void destroy_model(editor_t *ed)
{
    for (int row = 0; row < ROWS; row++)
        free(ed->data[row]);

    free(ed->yank);
    free(ed);
}

/**
 * data should have 1024 bytes allocated
 */
void screen_editor(context_t *ctx, int block, char *data)
{
    // Save current screen contents
    screen_t save_to;
    save_to.buffer = malloc(VGA_BUFSIZ);
    terminal_save(&save_to);

    hashtable_t *actions = actions_init();

    editor_t *ed = create_model(ctx, data);

    // Initial rendering
    draw_frame(block);
    render_model(ed);

    // Edit loop
    while (ed->render_op != MODEL_EXIT)
    {
        ed->render_op = MODEL_NONE;
        getchar_ext(&ed->input);
        render_model(process_key(actions, ed));
    }

    // TODO: Save/discard ability
    // Copy editor data lines into *data (join with \n)
    write_rows(data, ed);

    // Restore screen
    terminal_restore(&save_to);
    free(save_to.buffer);

    // Free alloc'd memory
    hashtable_destroy(actions);
    free(actions);
    destroy_model(ed);
}

