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
    printf("===============================================================================\n");
    printf("             ## Hulix: Forth machine, version: %s ##\n", VERSION);
    printf("     This program (c) 2014 Richard Hull, published under the MIT License\n");
    printf("    To read the licence, type LICENSE <enter>. For help, type HELP <enter>\n");
    printf("===============================================================================\n");
    printf("\n\nHello World\n");

    char *str = malloc(200);
    printf("string allocation = 0x%x\n", (unsigned int)str);
    free(str);

    //timer_wait(50);

    printf("Testing exceptions -- about to calculate 3 / 0 ...\n");
    for (int i = 0; i < 1000; i++) {
        printf("Line %d: %x, %d\n", i, 3000 - i, 3 / (700 - i));
    }
    printf("Now press some keys:\n");

    for(;;);
}
