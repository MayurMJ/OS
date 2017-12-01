#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kyb.h>
#include <sys/kernel_threads.h>

static uint8_t shift = 0;
static uint8_t control = 0;
static uint8_t display = 0;
static uint8_t bckspc = 0;

void kyb_irqhandler(void) {
	uint8_t scancode=0;
	char c = 0;
	display = 0;
	__asm__ __volatile("movq $0,%%rax\n\t"
			   "inb $0x60,%0\n\t"
			   :"=a"(scancode));
	if (scancode == BCKSPCDWN) {
		bckspc = 1;
	}
	else if (scancode == BCKSPCUP) {
		bckspc = 0;
	}
	else if (scancode == SHIFTDOWN) {
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
		if(shift == 1 ) {
			c = kyb_shift_map[scancode];
			display = 1;
		}
		else if (control == 1) {
			c = kyb_ctrl_map[scancode];
			display = 1;
		}
		else if (shift == 0 && control == 0)
		{
			c = kyb_map[scancode];
			display = 1;
		}
	}/*
	if (bckspc == 1) {
		if (TERM_BUF_OFFSET != 0)
			TERM_BUF_OFFSET--;
		char *addr = (char *)(TERMINAL_BUFFER + TERM_BUF_OFFSET);
		*addr = ' ';
	}*/
	//else if (display == 1) {
	if (display == 1) {
		char * videomem = (char*)(0xffffffff80000000 + 0xb8f6c);
		*videomem = c;
		videomem-=2;
		*videomem = ' ';
		if (control == 1) {
			*videomem = '^';
		}
/*		char *addr = (char *)(TERMINAL_BUFFER + TERM_BUF_OFFSET);
		*addr = c;
		// check for fg_task != null as input might arrive before a process has scheduled
		// a read from stdin
		if ((c == '\n') && (FG_TASK != NULL)) {
			FG_TASK->state = READY;
			FG_TASK = NULL;
		}*/
	}
}
