#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kyb.h>
#include <sys/kernel_threads.h>

static uint8_t shift = 0;
static uint8_t control = 0;
static uint8_t display_f = 0;
static uint8_t bckspc = 0;

void kyb_irqhandler(void) {
	uint8_t scancode=0;
	char c = 0;
	display_f = 0;
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
			display_f = 1;
		}
		else if (control == 1) {
			c = kyb_ctrl_map[scancode];
			display_f = 1;
		}
		else if (shift == 0 && control == 0)
		{
			c = kyb_map[scancode];
			display_f = 1;
		}
	}
	if (bckspc == 1) {// known bugs: if theres a tab bfr, it erases 1 space at a time
		/*
		if (TERM_BUF_OFFSET != 0)
			TERM_BUF_OFFSET--;
		char *addr = (char *)(TERMINAL_BUFFER + TERM_BUF_OFFSET);
		*addr = ' ';
		*/
		init_buffer();
                int error = put_stdin_into_buffer('\b');
                if (error == 0) local_echo();
	}
	else if (display_f == 1) {
		char * videomem = (char*)(0xffffffff80000000 + 0xb8f6c);
		*videomem = c;
		videomem-=2;
		*videomem = ' ';
		if (control == 1) {
			*videomem = '^';
		};
		init_buffer();
		int error = put_stdin_into_buffer(c);
		if (error == 0) local_echo();
/*		char *addr = (char *)(TERMINAL_BUFFER + TERM_BUF_OFFSET);
		*addr = c;
		// check for fg_task != null as input might arrive before a process has scheduled
		// a read from stdin
		if ((c == '\n') && (FG_TASK != NULL)) {
			FG_TASK->state = READY;
			FG_TASK = NULL;
		}
*/	}
}
