#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#undef __STDC_HOSTED__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t row;
    size_t column;
} position_t;

typedef struct {
    uint16_t *buffer;
    uint8_t color;
    position_t cursor_pos;
} screen_t;

typedef struct  // poor man's closure
{
    uint8_t (*fn)(char *token, char *line, int index, void *free_vars);
    void *free_vars;
} colorize_t;



extern void terminal_initialize(void);
extern void terminal_setcolor(uint8_t color);
extern void terminal_putchar(char c);
extern void terminal_write(const char* data, size_t size);
extern void terminal_writestring(const char* data);
extern void terminal_colorstring(const char *data, colorize_t *colorizer);
extern void terminal_clear(void);
extern void terminal_scroll(void);

extern void terminal_setcursor(position_t *position);
extern void terminal_getcursor(position_t *position);
extern void terminal_cursormode(uint8_t start, uint8_t end);
extern void terminal_decrementcursor(position_t *position);
extern uint16_t terminal_incrementcursor(position_t *position);
extern void terminal_flush(void);

extern void terminal_save(screen_t *screen);
extern void terminal_restore(screen_t *screen);

#define CURSOR_HIDE       terminal_cursormode(0x20, 0x00)
#define CURSOR_INSERT     terminal_cursormode(0x0E, 0x0F)
#define CURSOR_OVERWRITE  terminal_cursormode(0x00, 0x0F)

#ifdef __cplusplus
}
#endif

#endif
