#include <stdio.h>
#include <stdlib.h>
#include <kernel/system.h>
#include <kernel/tty.h>
#include <kernel/kb.h>

#define QUEUE_SIZE 128

/*set 1 keyboard output map*/
const char map[] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',   // 0x00 - 0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',   // 0x10 - 0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'z', 'x', 'c', 'v',   // 0x20 - 0x2F
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,                  // 0x30 - 0x3F
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',                // 0x40 - 0x4F
    '2', '3', '0', '.', 0, 0, '\\', 0                                                // 0x50 - 0x5F
};

/*set 1 keyboard output map to include shift function*/
const char shift_map[] =
{
    0, 27, '!', '"', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '~', 0, '~', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, '|', 0
};

/*set 1 keyboard output map to include caps functions*/
const char caps_map[] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, '\\', 0
};


typedef struct {
    char buffer[QUEUE_SIZE];
    volatile uint16_t read_index;
    volatile uint16_t write_index;
} queue_t;

#define capacity(q)   ((q.write_index - q.read_index + QUEUE_SIZE) % QUEUE_SIZE)

static queue_t q;

void keyboard_clear_buffer()
{
    cli();
    memset(q.buffer, 0, sizeof(char) * QUEUE_SIZE);
    q.read_index = 0;
    q.write_index = 0;
    sti();
}

void print_queue(queue_t *queue)
{
    position_t current;
    terminal_getcursor(&current);

    position_t origin = {0,0};
    terminal_setcursor(&origin);

    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        char c = queue->buffer[i];
        terminal_putchar(c < 0 ? '#' : c);
    }
    terminal_putchar('\n');
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (i == queue->read_index && i == queue->write_index)
        {
            terminal_putchar('*');
        }
        else if (i == queue->read_index)
        {
            terminal_putchar('R');
        }
        else if (i == queue->write_index)
        {
            terminal_putchar('W');
        }
        else
        {
            terminal_putchar(' ');
        }
    }
    terminal_setcursor(&current);
}

/* Puts c onto the queue: returns -1 if queue is full. */
char putch(char c)
{
    if (capacity(q) == 0 && q.buffer[q.write_index] != '\0')  // full?
    {
        return -1;
    }

    q.buffer[q.write_index] = c;
    q.write_index =  (q.write_index + 1) % QUEUE_SIZE;
    return c;
}

/* Returns the next character from the queue, or -1 if empty [NON-BLOCKING] */
char getch()
{
    if (capacity(q) == 0 && q.buffer[q.write_index] == '\0')  // empty?
    {
        return -1;
    }

    char c = q.buffer[q.read_index];
    q.buffer[q.read_index] = '\0';
    q.read_index = (q.read_index + 1) % QUEUE_SIZE;
    return c;
}

/* Returns the next character or blocks indefinitely */
char getchar()
{
    char c;
    while ((c = getch()) == -1)
    {
        __asm __volatile("sti; hlt");
    }
    return c;
}

static uint8_t extended = 0;
static uint8_t control = 0;
static uint8_t shift = 0;
static uint8_t caps = 0;

void keyboard_handler(registers_t *r)
{
    unsigned char scancode = inportb(0x60);
    //printf("r->int_no=%d,scancode=0x%x,caps=%d,shift=%d,control=%d,capacity=%d\n",
    //        r->int_no, scancode, caps, shift, control, capacity(q));

    if (scancode & 0x80)
    {
        switch (scancode)
        {
            case 0xAA:
            case 0xB6:
                shift = 0;
                break;
            case 0x9D:
                control = 0;
                break;
            case 0xE0:
                extended = 1;
                break;
            default:
                extended = 0;
                break;
        }
    }
    else if (scancode == 0x1D)  // Left CTRL
    {
        control = 1;
    }
    else if (scancode == 0x2A || scancode == 0x36) // Left/Right SHIFT
    {
        shift = 1;
    }
    else if (scancode == 0x3A) // CAPS lock
    {
        caps = caps == 0 ? 1 : 0;
    }
    else if (shift == 1 && caps == 0)
    {
        putch(shift_map[scancode]);
    }
    else if (shift == 1 && caps == 1)
    {
        putch(map[scancode]);
    }
    else if (caps == 1)
    {
        putch(caps_map[scancode]);
    }
    else
    {
        char c = map[scancode];
        if (control == 1 && c >= 'a' && c <= 'z')
        {
            // TODD: rather than convert, consider an int queue
            // and then uplift: c = 0x200 | c
            c -= 'a';
        }
        else if (extended == 1)
        {
            extended = 0;
            // TODD: rather than negate, consider an int queue
            // and then uplift: c = 0x100 | c
            c = -c;
        }

        putch(c);
    }
}

void keyboard_install()
{
    keyboard_clear_buffer();
    irq_install_handler(1, keyboard_handler);
}
