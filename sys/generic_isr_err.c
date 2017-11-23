#include <sys/kprintf.h>
#include <sys/defs.h>
//#include <sys/ahci.h>
void generic_irqhandler_err8(void)
{
    kprintf("Generic interrupt occured with error code 8\n");
	
}

void generic_irqhandler_err10(void)
{
    kprintf("Generic interrupt occured with error code 10\n");

}

void generic_irqhandler_err11(void)
{
    kprintf("Generic interrupt occured with error code 11\n");

}

void generic_irqhandler_err12(void)
{
    kprintf("Generic interrupt occured with error code 12\n");

}

void generic_irqhandler_err13(void)
{
    kprintf("Generic interrupt occured with error code 13\n");

}

void generic_irqhandler_err14(void)
{
    uint64_t cr2val;
    uint64_t cr3val;
    __asm__ __volatile__("movq %%cr2, %0\n\t"
                             :"=a"(cr2val)); 
    __asm__ __volatile__("movq %%cr3, %0\n\t"
                             :"=a"(cr3val)); 
    kprintf("page fault while accessing address %x\n", cr2val);
    //kprintf("cr3 %x", cr3val);
//    put_page_mapping(3, cr2val, cr3val);

}
