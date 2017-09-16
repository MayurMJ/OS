#include <sys/kprintf.h>
extern int timer_isr;
static int time_boot = 0;
void timer_irqhandler(void)
{
    kprintf("address of timer_isr %x\n",&timer_isr);
    kprintf("time since boot %d s \n",time_boot++);
}
