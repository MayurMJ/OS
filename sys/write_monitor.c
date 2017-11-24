#include <sys/kprintf.h>
#include <sys/defs.h>
//#include <sys/ahci.h>
void syscall_handler(void)
{
    uint64_t syscall_number=0;
    __asm__ __volatile__("movq %%rax, %0\n\t"
			:"=a" (syscall_number)
                        :);
    kprintf("Printing happening %d\n",syscall_number);
	
}
