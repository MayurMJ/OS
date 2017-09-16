#include <sys/idt.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
extern uint64_t timer_isr;
void init_idt() {
	int i = 0;
	uint16_t offset1=(uint16_t)((uint64_t)&timer_isr & 0x000000000000FFFF);	
	uint16_t offset2=(uint16_t)(((uint64_t)&timer_isr >> 16) & 0x000000000000FFFF);
	uint32_t offset3=(uint32_t)(((uint64_t)&timer_isr >> 32) & 0xFFFFFFFF);
	kprintf("%x, %x, %x, %x\n",  offset1, offset2, ~offset3, (uint64_t)&timer_isr);
	for(i = 0; i < 32; i++) {
		set_idt(&idt[i], 0, 8, 0, 0x8f, 0, 0);
	}
	for(i = 33; i < 256; i++) {
		set_idt(&idt[i], 0, 8, 0, 0x8e, 0, 0);	
	}
	set_idt(&idt[32], offset1, 8, 0, 0x8e, offset2, offset3);
		__asm__ __volatile("lidt (%0)"
			   :
            		   :"r"(idt)

             		);
}
void set_idt(struct IDTDescr *idt_entry, uint16_t offset_1, uint16_t selector, uint8_t ist, uint16_t type_attr, uint16_t offset_2, uint32_t offset_3){
	idt_entry->id_offset_1 	= offset_1;
	idt_entry->id_selector 	= selector;
	idt_entry->id_ist 	= ist;
	idt_entry->id_type_attr = type_attr;
	idt_entry->id_offset_2 	= offset_2;
	idt_entry->id_offset_3 	= offset_3;
	idt_entry->id_zero 	= 0;
}

