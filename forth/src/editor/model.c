#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editor/model.h>

editor_t *model_redraw(editor_t *ed, int from, int to)
{
    assert(from >= 0);
    assert(from <= to);
    assert(to < ROWS);

    ed->render_op = MODEL_REDRAW;
    ed->message = NULL;
    ed->redraw_start_row = from;
    ed->redraw_end_row = to;
    return ed;
}

editor_t *model_error(editor_t *ed, char *msg)
{
    ed->render_op = MODEL_ERROR;
    ed->message = msg;
    ed->redraw_start_row = -1;
    ed->redraw_end_row = -1;
    return ed;
}

