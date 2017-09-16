#include <sys/kprintf.h>
void generic_irqhandler(void)
{
    kprintf("Generic interrupt occured\n");
}
