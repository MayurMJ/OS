#include <sys/idt.h>
void init_idt() {
	__asm__ __volatile("lidt (%0)"
			   :
            		   :"r"(idt)

             		);
}

