#ifndef _IDT_H_
#define IDT_H_
#include <sys/defs.h>

typedef struct IDTDescr {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector;// = 8; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr;// = 14; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;// = 0;     // reserved
}idt_entry;
idt_entry idt[256];

void init_idt();

#endif
