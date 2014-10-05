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
    printf("Hulix: Forth interpreter\n");
    printf("\n\n\n\n\n\n\nHello World");

    for (int i = 0; i < 3000; i++) {
        printf("Line %d: %x\n", i, 3000 - i);
    }
}
