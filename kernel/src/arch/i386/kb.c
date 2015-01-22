#include <stdio.h>
#include <stdlib.h>
#include <kernel/system.h>
#include <kernel/tty.h>
#include <kernel/kb.h>
#include <kernel/kbd_map/en_gb.h>

#define QUEUE_SIZE 128

#define normal_map(kbd, scancode) (kbd[scancode])
#define shift_map(kbd, scancode)  (kbd[96 + scancode])
#define caps_map(kbd, scancode)   (kbd[192 + scancode])

#define capacity(q)   ((q.write_index - q.read_index + QUEUE_SIZE) % QUEUE_SIZE)

typedef struct {
    input_t buffer[QUEUE_SIZE];
    volatile uint16_t read_index;
    volatile uint16_t write_index;
} queue_t;

static queue_t q;
static char *kbd_map = (char *)&kbd_en_gb;
static flags_t *flags = { 0 };

void keyboard_clear_buffer()
{
    cli();
    memset(q.buffer, 0, sizeof(input_t) * QUEUE_SIZE);
    q.read_index = 0;
    q.write_index = 0;
    sti();
}

/* Puts c onto the queue: returns -1 if queue is full. */
char _putch(input_t c)
{
    if (capacity(q) == 0 && q.buffer[q.write_index].ascii != '\0')  // full?
    {
        return -1;
    }

    q.buffer[q.write_index] = c;
    q.write_index =  (q.write_index + 1) % QUEUE_SIZE;

    return c.ascii;
}

/**
 * Returns the next character from the queue, or -1 if empty [NON-BLOCKING].
 * The related input_t structure (scancode, flags, ascii value) is copied
 * into the supplied ptr
 */
char getch_ext(input_t *input)
{
    assert(input != NULL);
    if (capacity(q) == 0 && q.buffer[q.write_index].ascii == '\0')  // empty?
    {
        return -1;
    }

    memcpy(input, &q.buffer[q.read_index], sizeof(input_t));
    memset(&q.buffer[q.read_index], 0, sizeof(input_t));
    q.read_index = (q.read_index + 1) % QUEUE_SIZE;
    return input->ascii;
}

/**
 * Returns the next character or blocks indefinitely. The related input_t
 * structure (scancode, flags, ascii value) is copied into the supplied ptr
 */
char getchar_ext(input_t *input)
{
    char c;
    while ((c = getch_ext(input)) == -1)
    {
        __asm __volatile("sti; hlt");
    }
    return c;
}

/* Returns the next character from the queue, or -1 if empty [NON-BLOCKING] */
char getch()
{
    input_t inp;
    return getch_ext(&inp);
}

/* Returns the next character or blocks indefinitely */
char getchar()
{
    input_t inp;
    return getchar_ext(&inp);
}


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
                flags->shift = 0;
                return;
            case 0x9D:
                flags->control = 0;
                return;
            case 0xE0:
                flags->extended = 1;
                return;
            default:
                flags->extended = 0;
                return;
        }
    }

    if (scancode == 0x1D)  // Left CTRL
    {
        flags->control = 1;
        return;
    }

    if (scancode == 0x2A || scancode == 0x36) // Left/Right SHIFT
    {
        flags->shift = 1;
        return;
    }

    if (scancode == 0x3A) // CAPS lock
    {
        flags->capslock = ~flags->capslock;
        return;
    }

    char c;
    if (flags->shift && !flags->capslock)
    {
        c = shift_map(kbd_map, scancode);
    }
    else if (flags->shift && flags->capslock)
    {
        c = normal_map(kbd_map, scancode);
    }
    else if (flags->capslock)
    {
        c = caps_map(kbd_map, scancode);
    }
    else
    {
        c = normal_map(kbd_map, scancode);
        if (flags->control && c >= 'a' && c <= 'z')
        {
            // TODD: rather than convert, consider an int queue
            // and then uplift: c = 0x200 | c
            c -= 'a';
        }
        else if (flags->extended)
        {
            // TODD: rather than negate, consider an int queue
            // and then uplift: c = 0x100 | c
            c = -c;
        }
    }
    input_t input = { .flags = *flags, .scancode = scancode, .ascii = c };
    _putch(input);

    flags->extended = 0;
}

void keyboard_install()
{
    keyboard_clear_buffer();
    irq_install_handler(1, keyboard_handler);
}
