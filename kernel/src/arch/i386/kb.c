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

/* inserts the given character 'c' in 's' at the specified index, but
   only if there is enough space (according to sz). Returns true if
   successfully updated, else false if full. */
unsigned int insert(char *s, uint16_t index, char c, uint16_t sz)
{
    assert(index < sz);

    int len = strlen(s + index);
    if (index + len < sz - 1)  // still got one character of space we can insert into ?
    {
        // Shift the contents from index right one character
        memmove(s + index + 1, s + index, len);
        s[index] = c;
        return true;
    }
    else
    {
        // Dont insert anything and just return
        return false;
    }
}

/* removes the character at index - 1 in 's': used to implement 'backspace'
   style deletion where characters are consumed from the right. Returns
   true if characters were deleted, false otherwise (which occurs when
   trying to remove character at position zero. */
unsigned int remove(char *s, uint16_t index, uint16_t sz)
{
    assert(index < sz);

    if (index == 0)
    {
        return false;
    }
    else
    {
        char *dest = s + index - 1;
        char *src = s + index;
        memmove(dest, src, strlen(src) + 1);
        return true;
    }
}

/* removes the character at index in 's': used to implement 'delete' style
   deletion where characters are consumed from the left. */
uint16_t delete(char *s, uint16_t index, uint16_t sz)
{
    assert(index < sz);

    char *dest = s + index;
    char *src = s + index + 1;
    memmove(dest, src, strlen(src) + 1);
    return true;
}

uint16_t count_hist(char **history)
{
    uint16_t count = 0;
    while (*history++ != NULL)
        count++;

    return count;
}

void clear(position_t* posn, uint16_t count)
{
    terminal_setcursor(posn);
    for (int i = 0; i < count; i++)
        terminal_putchar(' ');
}

char *readline(char *buf, uint16_t sz, char **history)
{
    // TODO: tab completion
    // TODO: parens matching
    // TODO: Dont clear buf on start (i.e. allow default value)
    // TODO: Handle scrolling better

    terminal_flush();
    init_queue(&q);
    memset(buf, 0, sz);

    uint16_t index = 0;
    uint16_t len = strlen(buf);
    position_t start_posn;
    position_t cursor_posn;

    int hist_size = count_hist(history);
    int hist_index = hist_size > 0 ? hist_size : 0;

    terminal_getcursor(&start_posn);
    terminal_getcursor(&cursor_posn);

    while (1)
    {
        char c = getchar();

        if (c == KEY_LEFT && index > 0)
        {
            index--;
            terminal_decrementcursor(&cursor_posn);
            terminal_setcursor(&cursor_posn);
            continue;
        }
        else if (c == KEY_RIGHT && index < len)
        {
            index++;
            terminal_incrementcursor(&cursor_posn);
            terminal_setcursor(&cursor_posn);
            continue;
        }
        else if ((c == KEY_UP || c == KEY_DOWN) && hist_size > 0)
        {
            hist_index += (c == KEY_UP) ? -1 : 1;
            if (hist_index < 0)
                hist_index = hist_size -1;
            else if (hist_index >= hist_size)
                hist_index = 0;

            clear(&start_posn, len);
            char *src = history[hist_index];
            len = strlen(src);
            memset(buf, 0, sz);
            memcpy(buf, src, len + 1);

            while (index > len)
            {
                index--;
                terminal_decrementcursor(&cursor_posn);
            }
        }
        else if (c == KEY_CTRL_A || c == KEY_HOME)
        {
            for (uint16_t i = 0; i < index; i++)
            {
                terminal_decrementcursor(&cursor_posn);
            }
            index = 0;
        }
        else if (c == KEY_CTRL_E || c == KEY_END)
        {
            for (uint16_t i = index; i < len; i++)
            {
                terminal_incrementcursor(&cursor_posn);
            }
            index = len;
        }
        else if (c == KEY_DELETE)
        {
            if (index < sz && delete(buf, index, sz))
            {
                len--;
            }
        }
        else if (c == KEY_BACKSPACE)
        {
            if (index > 0 && remove(buf, index, sz))
            {
                index--;
                len--;
                terminal_decrementcursor(&cursor_posn);
            }
        }
        else if (c == KEY_NEW_LINE)
        {
            terminal_putchar('\n');
            break;
        }
        else if (c >= 32 && c <= 126 && index < sz - 1 && insert(buf, index, c, sz))
        {
            index++;
            len++;
            terminal_incrementcursor(&cursor_posn);
        }
        else if (c < 0) {
            //printf("     extended=%d  ", c);
        }

        terminal_setcursor(&start_posn);
        terminal_writestring(buf);
        terminal_putchar(' ');
        terminal_setcursor(&cursor_posn);
    }

    terminal_flush();
    return buf;
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
    init_queue(&q);
    irq_install_handler(1, keyboard_handler);
}
