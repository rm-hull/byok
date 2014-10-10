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
    printf("\n\nHello World\n");

    char *str = malloc(200);
    printf("string allocation = 0x%x\n", (unsigned int)str);
    free(str);

    printf("Keyboard and timer are active\n");
    timer_wait(100);

    printf("\nTesting printf -- %%d, %%x and %%f ...\n\n");
    for (int i = 0; i < 10; i++) {
        printf("  Line %d: 0x%x, sqrt(%d) = %f\n", i, 300 * i, i, sqrt(i));
    }

    timer_wait(100);

    printf("\nTesting exceptions -- about to calculate 3 / 0 ...\n\n");
    int x = 3;
    int y = 3 / (x - 3);
    printf("x = %d, y = %d\n", x, y);

    abort();
}
