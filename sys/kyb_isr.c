#include <sys/kprintf.h>
#include <sys/defs.h>

void kyb_irqhandler(void) {
	uint8_t scancode=0;
	__asm__ __volatile("movq $0,%%rax\n\t"
			   "inb $0x60,%0\n\t"
			   :"=a"(scancode));
	kprintf("scancode %d\n",scancode);
}
