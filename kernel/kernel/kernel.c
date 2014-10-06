#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

void kernel_early(void)
{
    terminal_initialize();
}

void kernel_main(void)
{
    printf("===============================================================================\n");
    printf("             ## Hulix: Forth machine, version: %s ##\n", VERSION);
    printf("     This program (c) 2014 Richard Hull, published under the MIT License\n");
    printf("    To read the licence, type LICENSE <enter>. For help, type HELP <enter>\n");
    printf("===============================================================================\n");
    printf("\n\nHello World");

    printf("Testing exceptions -- about to calculate 3 / 0 ...\n");
    for (int i = 0; i < 10; i++) {
        printf("Line %d: %x, %d\n", i, 3000 - i, 3 / (7 - i));
    }
}
