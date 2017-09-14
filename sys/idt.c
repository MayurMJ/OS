#include <sys/idt.h>
void init_idt() {
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

