#ifndef __KB_H
#define __KB_H

#include <stdio.h>
#include <stdlib.h>
#include <kernel/system.h>

// TODO: scancodes need to be looked up in the current keymap, rather
//       than hardcoded to the UK/US settings below
#define SCANCODE_CAPSLOCK     0x3a
#define SCANCODE_LEFT_ALT     0x38
#define SCANCODE_LEFT_CTRL    0x1d
#define SCANCODE_LEFT_SHIFT   0x2a
#define SCANCODE_RIGHT_SHIFT  0x36
#define SCANCODE_BACKSPACE    0x0e
#define SCANCODE_TAB          0x0f
#define SCANCODE_A            0x1e
#define SCANCODE_B            0x30
#define SCANCODE_D            0x20
#define SCANCODE_E            0x12
#define SCANCODE_F            0x21
#define SCANCODE_K            0x25
#define SCANCODE_T            0x14
#define SCANCODE_U            0x16
#define SCANCODE_W            0x11
#define SCANCODE_X            0x2d
#define SCANCODE_Y            0x15
#define SCANCODE_ENTER        0x1c
#define SCANCODE_DELETE       0x53
#define SCANCODE_INSERT       0x52
#define SCANCODE_END          0x4f
#define SCANCODE_HOME         0x47
#define SCANCODE_PGDN         0x51
#define SCANCODE_PGUP         0x49
#define SCANCODE_UP           0x48
#define SCANCODE_LEFT         0x4b
#define SCANCODE_RIGHT        0x4d
#define SCANCODE_DOWN         0x50

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t extended:1;
    uint8_t alt:1;
    uint8_t control:1;
    uint8_t shift:1;
    uint8_t capslock:1;
    uint8_t numlock:1;
} flags_t;

typedef struct {
    flags_t flags;
    char keycode;
    unsigned char scancode;
} input_t;

extern void keyboard_install();
extern void keyboard_clear_buffer();
extern char getch();
extern char getchar();
extern char getch_ext(input_t *input);
extern char getchar_ext(input_t *input);

#ifdef __cplusplus
}
#endif

#endif
