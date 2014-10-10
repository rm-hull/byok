#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>

void kernel_early(void)
{
    terminal_initialize();
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    __asm__ __volatile__ ("sti");
    timer_install();
    keyboard_install();
}

void kernel_main(void)
{
    draw_logo();
    printf("===============================================================================\n");
    terminal_setcolor(0x0F);
    printf("             ## BYOK: Forth machine, version: %s ##\n", VERSION);
    terminal_setcolor(0x07);
    printf("     This program (c) 2014 Richard Hull, published under the MIT License\n");
    printf("    To read the licence, type LICENSE <enter>. For help, type HELP <enter>\n");
    printf("===============================================================================\n");
    abort();
    printf("\n\nHello World\n");

    char *str = malloc(200);
    printf("string allocation = 0x%x\n", (unsigned int)str);
    free(str);

    //timer_wait(50);

    printf("Testing exceptions -- about to calculate 3 / 0 ...\n");
    for (int i = 0; i < 10; i++) {
        printf("Line %d: %x, %f\n", i, 3000 - i, 3.0 / (float)(700 - i));
    }
    abort();
    printf("Now press some keys:\n");

    for(;;);
}
