#include <kernel/system.h>

/* Keep track of how many ticks that the system has been running for */
static volatile unsigned long timer_ticks = 0;

/* Handles the timer by incrementing the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times per second. */
void timer_handler(registers_t *r)
{
    timer_ticks++;
}

/* Sets up the system clock by installing the timer handler into IRQ0 */
void timer_install()
{
    irq_install_handler(0, timer_handler);
}

/* Blocks until the given time has been reached */
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}
