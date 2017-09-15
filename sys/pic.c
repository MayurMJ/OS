#include <sys/defs.h>
// 0x20-master, 0xA0-slave
// 1-data, 0-cmd

void program_pic()
{
	unsigned char mask1, mask2;
	uint8_t offset1 = 0x20;
	uint8_t offset2 = 0x28;
	
	__asm__ __volatile("inb $0x21,%0"
			   :"=a"(mask1));

	__asm__ __volatile("inb $0xA1,%0"
                           :"=a"(mask2));
	
	__asm__ __volatile("movb $0x11,%%al\n\t"
			    "outb %%al,$0x20\n\t"
			    "outb %%al,$0xA0\n\t"
			    "movb %0,%%al\n\t"
			    "outb %%al,$0x21\n\t"
			    "movb %1,%%al\n\t"
                            "outb %%al,$0xA1\n\t"
                            "movb $4,%%al\n\t"
                            "outb %%al,$0x21\n\t"
			    "movb $2,%%al\n\t"
                            "outb %%al,$0xA1\n\t"
                            "movb $0x01,%%al\n\t"
                            "outb %%al,$0x21\n\t"
			    "outb %%al,$0xA1\n\t"
			    ::"a"(offset1), "b"(offset2));
			     
	__asm__ __volatile__("outb %0,$0x21"
			     ::"a"(mask1));

	__asm__ __volatile__("outb %0,$0xA1"
			     ::"a"(mask2));

	return;		
}

