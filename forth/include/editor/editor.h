#ifndef _EDITOR_H
#define _EDITOR_H

#include <stack_machine/context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void screen_editor(context_t *ctx, int block, char *data);

#ifdef __cplusplus
}
#endif

#endif
