#include <stdio.h>
#include <stdlib.h>
#include <kernel/system.h>
#include <kernel/tty.h>

#define QUEUE_SIZE 32
#define EXTEND_MARKER -2

/*set 1 keyboard output map*/
const char map[] =
{
    00, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 00, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 00, '#', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 00, '*', 00, ' ', 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 00, 00, 00, 00, 00
};

/*set 1 keyboard output map to include shift function*/
const char shift_map[] =
{
    00, 27, '!', '"', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 00, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '~', 00, '~', 'Z', 'X', 'C', 'V',
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
    char buffer[QUEUE_SIZE];
    volatile unsigned short read_index;
    volatile unsigned short write_index;
} queue_t;

#define capacity(q)   ((q.write_index - q.read_index + QUEUE_SIZE) % QUEUE_SIZE)

queue_t q;

void init_queue(queue_t *queue)
{
    cli();
    memset(queue->buffer, 0, sizeof(queue->buffer));
    queue->read_index = 0;
    queue->write_index = 0;
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
    //print_queue(&q);
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
    //print_queue(&q);
    return c;
}

/* Returns the next character or blocks indefinitely */
char getchar()
{
    char c;
    while ((c = getch()) == -1);
    return c;
}

unsigned int insert(char *s, unsigned int index, char c, unsigned int sz)
{
    assert(index < sz);

    int len = strlen(s + index);
    if (index + len < sz - 1)  // still got one character of space we can insert into ?
    {
        // Shift the contents from index right one character
        memmove(s + index + 1, s + index, len);
        s[index] = c;
        return index + 1;
    }
    else
    {
        // Dont insert anything and just return the current position
        return index;
    }
}

unsigned int remove(char *s, unsigned int index)
{
    if (index == 0)
    {
        return index;
    }
    else
    {
        char *dest = s + index - 1;
        char *src = s + index;
        memmove(dest, src, strlen(src) + 1);
        return index - 1;
    }
}

char *readline(char *buf, unsigned int sz)
{
    terminal_flush();
    init_queue(&q);
    unsigned int index = 0;

    memset(buf, 0, sz);

    position_t start_posn, cursor_posn;
    terminal_getcursor(&start_posn);
    terminal_getcursor(&cursor_posn);

    while (1)
    {
        char c = getchar();
        if (c == EXTEND_MARKER) // Extended code, and further code follows?
        {
            c = getchar();
            char discard = getchar();  // discard the next character
            assert(discard == EXTEND_MARKER);
            if (c == '4')   // Left arrow?
            {
                if (index > 0)
                {
                    index--;
                    terminal_decrementcursor(&cursor_posn);
                }
            }
            else if (c == '6')  // Right arrow?
            {
                if (index < strlen(buf))
                {
                    index++;
                    terminal_incrementcursor(&cursor_posn);
                }
            }
        }
        else if (c == '\b')
        {
            if (index > 0)
            {
                index = remove(buf, index);
                terminal_decrementcursor(&cursor_posn);
            }
        }
        else if (c == '\n' || c == '\r')
        {
            terminal_putchar('\n');
            break;
        }
        else if (index < sz - 1)
        {
            index = insert(buf, index, c, sz);
            terminal_incrementcursor(&cursor_posn);
        }

        terminal_setcursor(&start_posn);
        terminal_writestring(buf);
        terminal_putchar(' ');
        terminal_setcursor(&cursor_posn);
    }

    terminal_flush();
    return buf;
}

static int control = 0;
static int shift = 0;
static int caps = 0;


void keyboard_handler(registers_t *r)
{
    unsigned char scancode = inportb(0x60);
    //printf("r->int_no=%d,scancode=0x%x,caps=%d,shift=%d,control=%d,capacity=%d\n",
    //        r->int_no, scancode, caps, shift, control, capacity(q));

    if (scancode == 0xe0)  // Extended marker
    {
        putch(EXTEND_MARKER);
    }
    else if (scancode & 0x80)
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
    init_queue(&q);
    irq_install_handler(1, keyboard_handler);
}
