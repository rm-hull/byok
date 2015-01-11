#ifndef _COLORIZE_H
#define _COLORIZE_H 1

#include <kernel/vga.h>

#include <stack_machine/context.h>

#define COLOR_COMMENT         COLOR_LIGHT_BLUE
#define COLOR_LITERAL         COLOR_LIGHT_MAGENTA
#define COLOR_DEFN            COLOR_LIGHT_BROWN
#define COLOR_SYS_DICT_WORD   COLOR_BROWN
#define COLOR_USER_DICT_WORD  COLOR_LIGHT_GREEN
#define COLOR_DEFAULT         COLOR_LIGHT_GREY

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t colorize(char *token, char *line, int index, context_t *ctx);

#ifdef __cplusplus
}
#endif

#endif
