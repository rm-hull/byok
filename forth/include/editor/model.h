#ifndef _MODEL_H
#define _MODEL_H 1

#include <kernel/kb.h>
#include <stack_machine/context.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ROWS 16
#define COLUMNS 64
#define TABSTOPS 4

typedef enum { MODEL_REDRAW, MODEL_ERROR, MODEL_NONE, MODEL_EXIT } operation_t;
typedef enum { INSERT, OVERWRITE } inputmode_t;


typedef struct {
    int row;
    int col;
    char *data[ROWS];
    operation_t render_op;
    int redraw_start_row;
    int redraw_end_row;
    char *message;
    inputmode_t inputmode;
    input_t input;
    context_t *ctx;
} editor_t;


typedef struct {
    unsigned char scancode;
    editor_t *(*fn)(editor_t *ed);
} action_t;

extern editor_t *model_redraw(editor_t *ed, int from, int to);
extern editor_t *model_error(editor_t *ed, char *msg);

#ifdef __cplusplus
}
#endif

#endif
