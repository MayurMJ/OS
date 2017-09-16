#include <sys/kprintf.h>
extern int generic_isr;
void generic_irqhandler(void)
{
    kprintf("address of timer_isr %x\n",&timer_isr);
    kprintf("time since boot %d s \n",time_boot++);
}
