#ifndef _IDT_H_
#define IDT_H_
#include <sys/defs.h>



struct IDTDescr {
   uint16_t id_offset_1 :16; // offset bits 0..15
   uint16_t id_selector :16;// = 8; // a code segment selector in GDT or LDT
   uint8_t id_ist :3;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t id_type_attr : 8;// = 14; // type and attributes
   uint16_t id_offset_2 : 16; // offset bits 16..31
   uint32_t id_offset_3: 32; // offset bits 32..63
   uint32_t id_zero :32;// = 0;     // reserved
};
struct IDTDescr idt[256];

void init_idt();
void set_idt(struct IDTDescr *idt_entry, uint16_t offset_1, uint16_t selector, uint8_t ist, uint16_t type_attr, uint16_t offset_2, uint32_t offset_3);
#endif
