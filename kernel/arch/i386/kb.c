#include <stdio.h>
#include <kernel/system.h>
#include <kernel/tty.h>

#define MAX_SIZE 128

/*set 1 keyboard output map*/
const char map[] =
{
    00, 00, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 00, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 00, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 00, '*', 00, ' ', 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 00, 00, 00, 00, 00
};

/*set 1 keyboard output map to include shift function*/
const char shift_map[] =
{
    00, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 00, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 00, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 00, '*', 00, ' ', 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 00, 00, 00, 00, 00
};

/*set 1 keyboard output map to include caps functions*/
const char caps_map[] =
{
    00, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 00, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 00, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 00, '*', 00, ' ', 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 00, 00, 00, 00, 00
};

typedef struct {
    unsigned char buffer[MAX_SIZE];
    volatile unsigned short read_index;
    volatile unsigned short write_index;
} queue_t;

#define capacity(q)   ((q.write_index - q.read_index + MAX_SIZE) % MAX_SIZE)

static queue_t q;

void init_queue()
{
    cli();
    memset(q.buffer, 0, sizeof(q.buffer));
    q.read_index = 0;
    q.write_index = 0;
    sti();
}

void print_queue()
{
    terminal_putchar('\n');
    for (int i = 0; i < MAX_SIZE; i++)
    {
        terminal_putchar(q.buffer[i]);
    }
    terminal_putchar('\n');
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (i == q.read_index && i == q.write_index)
        {
            terminal_putchar('*');
        }
        else if (i == q.read_index)
        {
            terminal_putchar('R');
        }
        else if (i == q.write_index)
        {
            terminal_putchar('W');
        }
        else
        {
            terminal_putchar(' ');
        }
    }
    terminal_putchar('\n');
}

/* Returns the next character from the queue, or -1 if empty [NON-BLOCKING] */
char getch()
{
    terminal_flush();
    if (capacity(q) == 0 && q.buffer[q.write_index] == '\0')  // empty?
    {
        return -1;
    }

    char c = q.buffer[q.read_index];
    q.buffer[q.read_index] = '\0';
    q.read_index = (q.read_index + 1) % MAX_SIZE;
    //print_queue();
    terminal_putchar(c);
    terminal_flush();
    return c;
}

/* Returns the next character or blocks indefinitely */
char getchar()
{
    char c;
    while ((c = getch()) == -1);
    return c;
}

char *gets(char *buf, unsigned int sz)
{

    init_queue();
    char *s = buf;
    char *e = buf + sz -1;

    while (s < e)
    {
        char c = getchar();
        //printf("s = %x, e = %x, c = %d\n", s, e, c);
        if (c == '\n' || c == '\r') {
            break;
        }
        *s++ = c;
    }

    *s++ = '\0';
    return buf;
}
/* Puts c onto the queue: returns -1 if queue is full. */
unsigned char putch(unsigned char c)
{
    if (capacity(q) == 0 && q.buffer[q.write_index] != '\0')  // full?
    {
        return -1;
    }

    q.buffer[q.write_index] = c;
    q.write_index =  (q.write_index + 1) % MAX_SIZE;

    //print_queue();
    return c;
}

static int control = 0;
static int shift = 0;
static int caps = 0;

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
        }
    }
    else if (scancode == 0x1D)  // Left CTRL
    {
        control = 1;
    }
    else if (control == 1)
    {
        control = 0;
    }
    else if (scancode == 0x2A || scancode == 0x36) // Left/Right SHIFT
    {
        shift = 1;

    }
    else if (scancode == 0x3A) // CAPS lock
    {
        if (caps == 0)
        {
            caps = 1;
        }
        else
        {
            caps = 0;
        }
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
        putch(map[scancode]);
    }
}

void keyboard_install()
{
    init_queue();
    irq_install_handler(1, keyboard_handler);
}
