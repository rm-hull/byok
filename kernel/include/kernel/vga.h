#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_BUFSIZ (sizeof(uint16_t) * VGA_WIDTH * VGA_HEIGHT)

#define VGA_MEMORY (uint16_t*)0xB8000
#define CRT_CNTRL 0x3D4
#define CRT_DATA  (CRT_CNTRL + 1)

#define CRT_CURSOR_START_REG 0x0A
#define CRT_CURSOR_END_REG 0x0B
#define CRT_CURSOR_LOCN_HI 0x0E
#define CRT_CURSOR_LOCN_LO 0x0F

enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t make_vgaentry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

#define crt_controller_reg(addr, data) \
    outportb(CRT_CNTRL, addr);         \
    outportb(CRT_DATA, data);          \


#endif
