#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void terminal_initialize(void);
extern void terminal_setcolor(uint8_t color);
extern void terminal_putchar(char c);
extern void terminal_write(const char* data, size_t size);
extern void terminal_writestring(const char* data);
extern void terminal_clear(void);
extern void terminal_scroll(void);
extern void terminal_setcursor(int column, int row);
extern void terminal_flush(void);

#ifdef __cplusplus
}
#endif

#endif
