#include <sys/kprintf.h>
#include <sys/scheduler.h>
//#include <sys/defs.h>
//#include <sys/ahci.h>
void generic_irqhandler(void)
{
    kprintf("Generic interrupt occured\n");
	kprintf("This was bad, but gracefully exit\n");
                reparent_orphans(CURRENT_TASK);
                CURRENT_TASK->state = ZOMBIE;
                CURRENT_TASK->exit_value = -1;
//                display_queue();
                schedule();
	
}
