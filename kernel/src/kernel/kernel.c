#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/system.h>
#include <math.h>

#include <stack_machine/repl.h>

void kernel_early(void)
{
    //mmu_install();
    terminal_initialize();
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    __asm__ __volatile__ ("sti");
    timer_install();
    keyboard_install();
    draw_logo();
}

void kernel_main(void)
{
    repl();
/*    char *buf = malloc(200);
    printf("string allocation = 0x%x\n", (unsigned int)buf);

    printf("\n\nKeyboard is active\nEnter some text & press return: ");
    readline(buf, 200);
    printf("You entered: '%s' (len=%d)\n", buf, strlen(buf));
    assert(strlen(buf) != 18);

    char *token = strtok(buf, "\t\n ");
    if (token != NULL)
        printf("Tokens:\n");

    while (token != NULL)
    {
        printf("  %s\n", token);
        token = strtok(NULL, "\t\n ");
    }


    printf("Timer is active - pausing for 5 seconds...\n");
    timer_wait(5 * 18);

    printf("Enter some more text: ");
    readline(buf, 200);
    printf("You entered: '%s' (len=%d)\n", buf, strlen(buf));
    free(buf);


    printf("\nTesting printf -- %%d, %%x and %%f ...\n\n");
    for (int i = 0; i < 10; i++) {
        printf("  Line %d: 0x%x, sqrt(%d) = %f\n", i, 300 * i, i, sqrt(i));
    }

    printf("\nTesting exceptions -- about to calculate 3 / 0 ...\n\n");
    timer_wait(3 * 18);

    int x = 3;
    int y = 3 / (x - 3);
    printf("x = %d, y = %d\n", x, y);

    abort();*/
}
