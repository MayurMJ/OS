#include <sys/defs.h>
// 0x20-master, 0xA0-slave
// 1-data, 0-cmd

void program_pic()
{
	uint8_t mask1=0, mask2=0;
//	uint8_t offset1 = 0x20;
//	uint8_t offset2 = 0x28;
	
	__asm__ __volatile("movq $0,%%rax\n\t"
			   "inb $0x21,%0\n\t"
			   :"=a"(mask1));
//	__asm__ __volatile("nop");
	__asm__ __volatile("movq $0,%%rax\n\t"
			   "inb $0xA1,%0\n\t"
                           :"=a"(mask2));
//	__asm__ __volatile("nop");
	__asm__ __volatile("movb $0x11,%%al\n\t"
			    "outb %%al,$0x20\n\t"
			    "movb $0x11,%%al\n\t"
			    "outb %%al,$0xA0\n\t"
			    "movb $0x20,%%al\n\t"
			    "outb %%al,$0x21\n\t"
			    "movb $0x28,%%al\n\t"
                            "outb %%al,$0xA1\n\t"
                            "movb $4,%%al\n\t"
                            "outb %%al,$0x21\n\t"
			    "movb $2,%%al\n\t"
                            "outb %%al,$0xA1\n\t"
                            "movb $0x01,%%al\n\t"
                            "outb %%al,$0x21\n\t"
			    "movb $0x01,%%al\n\t"
			    "outb %%al,$0xA1\n\t"
			    ::);
			    //::"a"(offset1), "b"(offset2));
//	__asm__ __volatile("nop");
	__asm__ __volatile(
			     "outb %0,$0x21\n\t"
			     ::"a"(mask1));
//	__asm__ __volatile("nop");
	__asm__ __volatile(
			     "outb %0,$0xA1\n\t"
			     ::"a"(mask2));
//	__asm__ __volatile("nop");
	return;		
}

