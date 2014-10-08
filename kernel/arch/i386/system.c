#include <stdlib.h>
#include <kernel/system.h>

/* Read/write to I/O ports to read/send bytes to devices.
   Inline assembly for the stuff that cannot be done in C */

unsigned char inportb (uint16_t port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outportb (uint16_t port, unsigned char data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

