#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/system.h>

size_t terminal_row;
size_t terminal_column;
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
    terminal_row = 0;
    terminal_column = 0;

    uint16_t blank = make_vgaentry(' ', terminal_color);
    for ( size_t index = 0; index < VGA_WIDTH * VGA_HEIGHT; index++ )
    {
        terminal_buffer[index] = blank;
    }
}

void terminal_scroll(void)
{
    terminal_row = VGA_HEIGHT - 1;
    memmove((void*)terminal_buffer, (void*)(terminal_buffer + VGA_WIDTH),
           sizeof(uint16_t) * VGA_HEIGHT * (VGA_WIDTH - 1));

    uint16_t blank = make_vgaentry(' ', terminal_color);
    for ( size_t x = 0; x < VGA_WIDTH; x++ )
    {
        const size_t index = terminal_row * VGA_WIDTH + x;
        terminal_buffer[index] = blank;
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
    switch (c)
    {
        case 0x08:  // Backspace
            if ( terminal_column > 0 )
            {
                terminal_column--;
                terminal_setcursor(terminal_column, terminal_row);
            }
            break;

        case 0x09:  // Tab
            terminal_column = (terminal_column + 8) & ~7;
            break;

        case '\r':  // Carriage Return
            terminal_column = 0;
            terminal_setcursor(terminal_column, terminal_row);
            break;

        case '\n':   // Line Feed
            terminal_column = 0;
            terminal_row++;
            terminal_setcursor(terminal_column, terminal_row);
            break;

        default:
            terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
            terminal_column++;
            break;
    }

    if ( terminal_column >= VGA_WIDTH )
    {
        terminal_column = 0;
        terminal_row++;
        terminal_setcursor(terminal_column, terminal_row);
    }

    if ( terminal_row >= VGA_HEIGHT )
    {
        terminal_scroll();
        terminal_setcursor(terminal_column, terminal_row);
    }
}

void terminal_write(const char* data, size_t size)
{
    for ( size_t i = 0; i < size; i++ )
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

void terminal_setcursor(int column, int row)
{
    unsigned int index = (row * VGA_WIDTH) + column;

    outportb(CRT_CNTRL, 14);
    outportb(CRT_CNTRL + 1, index >> 8);
    outportb(CRT_CNTRL, 15);
    outportb(CRT_CNTRL + 1, index);
}
