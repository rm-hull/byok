#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/system.h>

screen_t* console;

void terminal_initialize(void)
{
    if (console != NULL)
        free(console);

    console = (screen_t *)malloc(sizeof(screen_t));
    if (console != NULL) {
        console->buffer = VGA_MEMORY;
        console->color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        terminal_clear();
    }
}

void terminal_clear(void)
{
    console->cursor_pos.row = 0;
    console->cursor_pos.column = 0;

    uint16_t blank = make_vgaentry(' ', console->color);
    for ( size_t index = 0; index < VGA_WIDTH * VGA_HEIGHT; index++ )
    {
        console->buffer[index] = blank;
    }

    terminal_flush();
}

void terminal_scroll(void)
{
    console->cursor_pos.row = VGA_HEIGHT - 1;
    memmove((void*)console->buffer, (void*)(console->buffer + VGA_WIDTH),
           sizeof(uint16_t) * VGA_HEIGHT * (VGA_WIDTH - 1));

    uint16_t blank = make_vgaentry(' ', console->color);
    for ( size_t x = 0; x < VGA_WIDTH; x++ )
    {
        const size_t index = console->cursor_pos.row * VGA_WIDTH + x;
        console->buffer[index] = blank;
    }

    terminal_flush();
}

void terminal_setcolor(uint8_t color)
{
    console->color = color;
}

static inline void terminal_putentryat(char c, uint8_t color, position_t *position)
{
    const size_t index = position->row * VGA_WIDTH + position->column;
    console->buffer[index] = make_vgaentry(c, color);
}

/* Move the cursor onto the next position, returning 1 if a vertical scroll is necessary */
uint16_t terminal_incrementcursor(position_t *position)
{
    position->column++;
    if (position->column >= VGA_WIDTH)
    {
        position->column = 0;
        position->row++;
    }

    if (position->row >= VGA_HEIGHT)
    {
        position->row = VGA_HEIGHT-1;
        return 1;
    }

    return 0;
}

void terminal_decrementcursor(position_t *position)
{
    if ( position->column == 0 )
    {
        position->row--;
        position->column = VGA_WIDTH - 1;
    }
    else
    {
        position->column--;
    }
}
void terminal_putchar(char c)
{
    switch (c)
    {
        case '\b':  // Backspace
            terminal_decrementcursor(&console->cursor_pos);
            terminal_putentryat(' ', console->color, &console->cursor_pos);
            terminal_flush();
            break;

        case '\t':  // Tab
            console->cursor_pos.column = (console->cursor_pos.column + 8) & ~7;
            break;

        case '\r':  // Carriage Return
            console->cursor_pos.column = 0;
            terminal_flush();
            break;

        case '\n':   // Line Feed
            console->cursor_pos.column = 0;
            console->cursor_pos.row++;
            terminal_flush();
            break;

        default:
            terminal_putentryat(c, console->color, &console->cursor_pos);
            console->cursor_pos.column++;
            break;
    }

    if ( console->cursor_pos.column >= VGA_WIDTH )
    {
        console->cursor_pos.column = 0;
        console->cursor_pos.row++;
        terminal_flush();
    }

    if ( console->cursor_pos.row >= VGA_HEIGHT )
    {
        terminal_scroll();
    }
}

void terminal_write(const char* data, size_t size)
{
    for ( size_t i = 0; i < size; i++ )
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    char c, *s = (char *)data;
    while ((c = *s++) != 0)
    {
        terminal_putchar(c);
    }
}

/* Copy the current cursor position into the supplied position */
extern void terminal_getcursor(position_t *position)
{
    position->row = console->cursor_pos.row;
    position->column = console->cursor_pos.column;
}

void terminal_setcursor(position_t* position)
{
    console->cursor_pos.row = position->row;
    console->cursor_pos.column = position->column;

    unsigned int index = (position->row * VGA_WIDTH) + position->column;
    crt_controller_reg(CRT_CURSOR_LOCN_HI, index >> 8);
    crt_controller_reg(CRT_CURSOR_LOCN_LO, index);
}

/**
 * See:
 *   http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
 *   http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0B
 *
 * For full block, call: terminal_cursormode(0, 0x0F);
 *     underscore, call: terminal_cursormode(0x0E, 0x0F);
 * To hide:              terminal_cursormode(0x20, 0x00);
 */
void terminal_cursormode(uint8_t start, uint8_t end)
{
    crt_controller_reg(CRT_CURSOR_START_REG, start);
    crt_controller_reg(CRT_CURSOR_END_REG, end);
}


void terminal_flush()
{
    terminal_setcursor(&console->cursor_pos);
}

/* Populates to_buffer from the current VGA console; Callers responsibility
   to ensure that the struct and its buffer is malloc'd first */
void terminal_save(screen_t *save_to)
{
    if (save_to != NULL && save_to->buffer != NULL)
    {
        assert(save_to->buffer != console->buffer);

        save_to->color = console->color;
        save_to->cursor_pos.row = console->cursor_pos.row;
        save_to->cursor_pos.column = console->cursor_pos.column;
        memcpy(save_to->buffer, console->buffer, VGA_BUFSIZ);
    }
}

void terminal_restore(screen_t *restore_from)
{
    if (restore_from != NULL && restore_from->buffer != NULL)
    {
        assert(restore_from->buffer != console->buffer);

        console->color = restore_from->color;
        console->cursor_pos.row = restore_from->cursor_pos.row;
        console->cursor_pos.column = restore_from->cursor_pos.column;
        memcpy(console->buffer, restore_from->buffer, VGA_BUFSIZ);
        terminal_flush();
    }
    else
    {
        // No restore_from buffer, so just clear the screen
        terminal_clear();
    }
}
