#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/idt.h>
#include <sys/kernel_threads.h>
#include <sys/kmalloc.h>
#include <sys/elf64.h>
#include <sys/paging.h>
#include <sys/kmemcpy.h>
#include <sys/scheduler.h>
void put_in_run_queue(Task *newtask) {
	if (run_queue == NULL)
		run_queue = newtask;
	else {
		newtask->next = run_queue;
		run_queue->prev = newtask;
                run_queue = newtask;
	}
	newtask->pid = (last_assn_pid+1)%MAX_PROC;
	last_assn_pid = newtask->pid;
	return;
}
/*
uint64_t execve(char *binary, char *argv[], char *envp[]) {
        // returns a new task struct with newcr3
        Task *newtask = loadElf(binary);
        uint64_t oldcr3;
        __asm__ __volatile__("movq %%cr3, %0\n\t"
                             :"=a"(oldcr3));
        // switch to new cr3
        __asm__ __volatile__("movq %0, %%cr3\n\t"
                             ::"a"(newtask->regs.cr3));
        // free all old pages
        free_old_page_tables(oldcr3+0xffffffff80000000);
        return 0;
}
*/
