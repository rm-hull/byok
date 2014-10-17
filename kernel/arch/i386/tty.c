#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/system.h>


position_t terminal_position;
uint8_t terminal_color;
uint16_t* terminal_buffer;


void terminal_initialize(void)
{
    terminal_buffer = VGA_MEMORY;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_clear();
}

void terminal_clear(void)
{
    terminal_position.row = 0;
    terminal_position.column = 0;

    uint16_t blank = make_vgaentry(' ', terminal_color);
    for ( size_t index = 0; index < VGA_WIDTH * VGA_HEIGHT; index++ )
    {
        terminal_buffer[index] = blank;
    }

    terminal_flush();
}

void terminal_scroll(void)
{
    terminal_position.row = VGA_HEIGHT - 1;
    memmove((void*)terminal_buffer, (void*)(terminal_buffer + VGA_WIDTH),
           sizeof(uint16_t) * VGA_HEIGHT * (VGA_WIDTH - 1));

    uint16_t blank = make_vgaentry(' ', terminal_color);
    for ( size_t x = 0; x < VGA_WIDTH; x++ )
    {
        const size_t index = terminal_position.row * VGA_WIDTH + x;
        terminal_buffer[index] = blank;
    }

    terminal_flush();
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

static inline void terminal_putentryat(char c, uint8_t color, position_t *position)
{
    const size_t index = position->row * VGA_WIDTH + position->column;
    terminal_buffer[index] = make_vgaentry(c, color);
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
            terminal_decrementcursor(&terminal_position);
            terminal_putentryat(' ', terminal_color, &terminal_position);
            terminal_flush();
            break;

        case '\t':  // Tab
            terminal_position.column = (terminal_position.column + 8) & ~7;
            break;

        case '\r':  // Carriage Return
            terminal_position.column = 0;
            terminal_flush();
            break;

        case '\n':   // Line Feed
            terminal_position.column = 0;
            terminal_position.row++;
            terminal_flush();
            break;

        default:
            terminal_putentryat(c, terminal_color, &terminal_position);
            terminal_position.column++;
            break;
    }

    if ( terminal_position.column >= VGA_WIDTH )
    {
        terminal_position.column = 0;
        terminal_position.row++;
        terminal_flush();
    }

    if ( terminal_position.row >= VGA_HEIGHT )
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
    char c, *s = data;
    while ((c = *s++) != 0)
    {
        terminal_putchar(c);
    }
}

/* Copy the current cursor position into the supplied position */
extern void terminal_getcursor(position_t *position)
{
    position->row = terminal_position.row;
    position->column = terminal_position.column;
}

void terminal_setcursor(position_t* position)
{
    terminal_position.row = position->row;
    terminal_position.column = position->column;

    unsigned int index = (position->row * VGA_WIDTH) + position->column;
    outportb(CRT_CNTRL, 0x0E);
    outportb(CRT_CNTRL + 1, index >> 8);
    outportb(CRT_CNTRL, 0x0F);
    outportb(CRT_CNTRL + 1, index);
}

void terminal_flush()
{
    terminal_setcursor(&terminal_position);
}

