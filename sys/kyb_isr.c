#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kyb.h>
#define SHIFTDOWN	42
#define SHIFTUP		170
#define CONTROLDOWN	29
#define CONTROLUP	157

static uint8_t shift = 0;
static uint8_t control = 0;
static uint8_t display = 0;
void kyb_irqhandler(void) {
	uint8_t scancode=0;
	char c = 0;
	display = 0;
	__asm__ __volatile("movq $0,%%rax\n\t"
			   "inb $0x60,%0\n\t"
			   :"=a"(scancode));
	if (scancode == SHIFTDOWN) {
		shift = 1;
	}
	else if (scancode == SHIFTUP) {
		shift = 0;
	}
	else if (scancode == CONTROLDOWN) {
                control = 1;
        }
	else if (scancode == CONTROLUP) {
                control = 0;
        }
	else if (scancode<128) {
		if(shift == 1 || control == 1) {
			c = kyb_shift_map[scancode];
			display = 1;
		}
		else if (shift == 0)
		{
			c = kyb_map[scancode];
			display = 1;
		}
	}
	if (display == 1) {
		char * videomem = (char*)0xb8f6c;
		*videomem = c;
		videomem-=2;
		*videomem = ' ';
		if (control == 1) {
			*videomem = '^';
		}
	}
}
