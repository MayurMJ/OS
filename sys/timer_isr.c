#include <sys/kprintf.h>
static int time_boot = 0;
void timer_irqhandler(void)
{
    kprintf("time since boot %d s \n",time_boot++);
}
