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

extern void terminal_initialize(void);
extern void terminal_setcolor(uint8_t color);
extern void terminal_putchar(char c);
extern void terminal_write(const char* data, size_t size);
extern void terminal_writestring(const char* data);
extern void terminal_clear(void);
extern void terminal_scroll(void);

extern void terminal_setcursor(position_t *position);
extern void terminal_getcursor(position_t *position);
extern void terminal_decrementcursor(position_t *position);
extern uint16_t terminal_incrementcursor(position_t *position);
extern void terminal_flush(void);

#ifdef __cplusplus
}
#endif

#endif
