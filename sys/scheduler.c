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
#include <sys/initfs.h>
#include <sys/gdt.h>
#include <sys/copy_tables.h>

static Task *run_queue;;
static Task *queue_head;;
static Task *schedulerTask;


//fills up the task registers and the kstack pointer CAUTION::check the cr3, must set it appropriately after using this function
void setupTask(Task *task, void (*main)(), Task *otherTask) {
    task->regs.rax = 0;
    task->regs.rbx = 0;
    task->regs.rcx = 0;
    task->regs.rdx = 0;
    task->regs.rsi = 0;
    task->regs.rdi = 0;
    task->regs.rflags = otherTask->regs.rflags;
    task->regs.rip = (uint64_t) main;
    task->regs.cr3 = otherTask->regs.cr3;
    task->regs.r8 = 0;
    task->regs.r9 = 0;
    task->regs.r10 = 0;
    task->regs.r11 = 0;
    task->regs.r12 = 0;
    task->regs.r13 = 0;
    task->regs.r14 = 0;
    task->regs.r15 = 0;
    task->regs.rsp = (uint64_t) (4088 + get_free_page(SUPERVISOR_ONLY, task->regs.cr3)); // since it grows downward
    task->kstack = (uint64_t *)task->regs.rsp;//(4088 + get_free_page(SUPERVISOR_ONLY, task->regs.cr3)); // since it grows downward
}

void switch_user_mode(uint64_t symbol) {
        uint64_t oldcr3;
        __asm__ __volatile__("movq %%cr3, %0\n\t"
                             :"=a"(oldcr3));
        kprintf("\ncr3val:%x\n ", oldcr3 );
        __asm__ __volatile__ ( "cli\n\t"
                        "movq %1, %%rsp\n\t"
                        "movw $0x23, %%ax\n\t"
                        "movw %%ax, %%ds\n\t"
                        "movw %%ax, %%es\n\t"
                        "movw %%ax, %%fs\n\t"
                        "movw %%ax, %%gs\n\t"
                        "movq %%rsp, %%rax\n\t"
                        "pushq $0x23\n\t"
                        "pushq %%rax\n\t"
                        "pushfq\n\t"
                        "popq %%rax\n\t"
                        "orq $0x200, %%rax\n\t"
                        "pushq %%rax\n\t"
                        "pushq $0x2B\n\t"
                        "push %0\n\t"
                        "iretq\n\t"
                        ::"b"(symbol), "c"(CURRENT_TASK->mm->stack_begin)
        );
}
#define DEBUG_PRINT_SCHEDULER
void display_queue() {
#ifdef DEBUG_PRINT_SCHEDULER
	Task * curr = queue_head->next;
	while(curr != queue_head) {
		kprintf("%d(%d) -> ", curr->pid,curr->state);
		curr = curr->next;
	}
	kprintf("%d(%d) \n", curr->pid,curr->state);
#endif
}

void display_pid() {
        Task * curr = queue_head->next;
	kprintf("\nPID\tSTATE\tPPID\n");
        while(curr != queue_head) {
                kprintf("%d\t", curr->pid);
		if(curr->state == READY ) kprintf("READY\t");
		if(curr->state == WAITING ) kprintf("WAITING\t");
		if(curr->state == ZOMBIE ) kprintf("ZOMBIE\t");
		if(curr->state == SLEEP ) kprintf("SLEEP\t");
                kprintf("%d\n", curr->ppid);
		
                curr = curr->next;
        }
        //kprintf("%d(%d) \n", curr->pid,curr->state);
}


void put_in_run_queue(Task *newTask) {
	newTask->next = run_queue;
	queue_head->next = newTask;
	run_queue = newTask; 
	/*
	run_queue->next = newTask;
	newTask->next = queue_head;
	run_queue = newTask;*/
	return;
}


/* remove the input task from the queue, doesn't free the memory though */
void remove_from_run_queue(Task * removeTask) {
	if(removeTask == queue_head) {
		kprintf("PANIC: trying to remove the idle task\n");
		return;
	}
	if(removeTask == run_queue) {
		run_queue = removeTask->next;
		queue_head->next = run_queue;
	}
	else {
		Task * curr = run_queue;
		while(curr->next!= removeTask && curr != queue_head) {
			curr = curr->next;
		}
		if(curr == queue_head) {
			kprintf("PANIC: trying to remove a task which doesn't exist\n");
		}
		else {
			curr->next = removeTask->next;	
		}	
	}
	return;
}


void yield() {
    Task *last = CURRENT_TASK;
    
    CURRENT_TASK = CURRENT_TASK->next;
    while(CURRENT_TASK->state != READY) {
#ifdef DEBUG_PRINT_SCHEDULER
	kprintf("this task is not ready, skipping  pid = %d\n", CURRENT_TASK->pid);
    kprintf("scheduling pid %d\n",CURRENT_TASK->pid);
#endif
    	CURRENT_TASK = CURRENT_TASK->next;
    }
//		display_queue();
    set_tss_rsp((void *)((uint64_t)CURRENT_TASK->kstack));
    switchTask(&last->regs, &CURRENT_TASK->regs);
}


void schedule(){
//	Task * curr = CURRENT_TASK;
//	Task * next = run_queue;
	yield();	

}
//#define DEBUG_PRINT_SCHEDULER 1
void idle_task() {
	while(1) {
#ifdef DEBUG_PRINT_SCHEDULER
		kprintf("In the idle task, will stay here forever unless a new thread is available to schedule\n");
		display_queue();
#endif
		kprintf("In the idle task, will stay here forever unless a new thread is available to schedule\n");
		schedule();
		reap_all_child(CURRENT_TASK);
//		__asm__ __volatile__ ( "sti\n\t");
//		__asm__ __volatile__("hlt\n\t");
	}
}

void bin_init_user() {
//	while(1){
		kprintf("bin init kernel thread\n");
//		schedule();
//	}
	set_tss_rsp((void *)((uint64_t)CURRENT_TASK->kstack));
	switch_user_mode(CURRENT_TASK->mm->e_entry);	
}


// initialize the run queue with idle task and bin init
void init_scheduler() {
	schedulerTask = CURRENT_TASK;
	last_assn_pid = 0;
	run_queue = NULL;

	//idle task setup and made queue head
	Task *idleTask = (Task *)kmalloc(sizeof(Task));
	setupTask(idleTask,idle_task,schedulerTask);
	idleTask->pid = (last_assn_pid+1)%MAX_PROC;
	idleTask->ppid = 1;	//making the idle task it's own parent, just in case
	last_assn_pid = idleTask->pid;
	idleTask->state = READY;

	run_queue = idleTask;
	queue_head = idleTask;
	run_queue->next = queue_head;

	
	/*
	char *s1 = "/rootfs/bin/sample";
	char *argv[2];
        argv[0] = s1;
        argv[1] = '\0';
	char *s2 = "param2";
	char *argv[3];
	argv[0] = s1;
	argv[1] = s2;
	argv[2] = '\0'; */
	//Task *binInit = loadElf("bin/init", argv, NULL);
	Task *binInit = loadElf("bin/init", NULL, NULL);
	if(binInit == NULL) {
		kprintf("Could not find the file to load from elf!\n");
		//kernel panic should happen
	}
	
	setupTask(binInit,bin_init_user,idleTask);
	binInit->regs.cr3 = binInit->mm->pg_pml4;
	put_in_run_queue(binInit);
	binInit->state = READY;
	binInit->ppid = idleTask->pid;
/*
	Task *c = (Task *)kmalloc(sizeof(Task));
	c->pid = (last_assn_pid+1)%MAX_PROC;
	last_assn_pid = c->pid;
	c->state = READY;
	put_in_run_queue(c);
	c = (Task *)kmalloc(sizeof(Task));
	c->pid = (last_assn_pid+1)%MAX_PROC;
	last_assn_pid = c->pid;
	c->state = READY;
	put_in_run_queue(c);
	c = (Task *)kmalloc(sizeof(Task));
	c->pid = (last_assn_pid+1)%MAX_PROC;
	last_assn_pid = c->pid;
	c->state = READY;
	put_in_run_queue(c);
	c = (Task *)kmalloc(sizeof(Task));
	c->pid = (last_assn_pid+1)%MAX_PROC;
	last_assn_pid = c->pid;
	c->state = READY;
	put_in_run_queue(c);
	display_queue();
	while(1);*/
}


void scheduler() {
	kprintf("Welcome to scheduler!\n");
	init_scheduler();
	CURRENT_TASK = run_queue;
//	run_queue = run_queue->next;
        set_tss_rsp((void *)((uint64_t)CURRENT_TASK->kstack));
	switchTask(&schedulerTask->regs, &CURRENT_TASK->regs);		
	while(1);
}

/* returns true if the input task has a child, 0 otherwise */
uint64_t has_child(Task * parent) {
	Task * curr = run_queue;
	while(curr!=queue_head) {
		if(curr->ppid == parent->pid)
			return 1;
		curr = curr->next;
	}
	return 0;
}


/* returns a zombie child task of the input parent if it exists, NULL otherwise*/
Task* zombie_child_exists(Task * parent) {
	Task * curr = run_queue;
        while(curr!=queue_head) {
                if(curr->ppid == parent->pid && curr->state == ZOMBIE) { //curr is a child of the parent and it's state is ZOMBIE
                        return curr;
		}
                curr = curr->next;
        }
        return NULL;
}


/*reap all the children of the parent task */

void reap_all_child(Task *parent) {
	Task * curr = run_queue;
	while(curr!=queue_head) {
		if(curr->ppid == parent->pid && curr->state == ZOMBIE) {
			Task * reapme = curr;
			curr = curr->next;
			reap_process(reapme);
		}
		else {
			curr = curr->next;
		}
	}
//	display_queue();

}

void free_vmas(struct vma *vm_begin) {
	if(!vm_begin)
		return;
	struct vma *vm_curr = vm_begin;
	while(vm_curr->vma_next) {
		struct vma * deleteme = vm_curr;
		vm_curr = vm_curr->vma_next;
		kfree((uint64_t *)deleteme);
	} 
	kfree((uint64_t *)vm_curr);
}


void free_file_desc(Task * reapThis) {
	for(int i = 0; i < MAX_FDS; i++) {
		if(reapThis->file_desc[i]) {
			reapThis->file_desc[i]->file_ref_count--;
			if (reapThis->file_desc[i]->file_ref_count <= 0)
				kfree((uint64_t *)(reapThis->file_desc[i]));
		}
	}

}

/* reap the process */
void reap_process(Task * reapThis) {
	remove_from_run_queue(reapThis); 
	free_vmas(reapThis->mm->vm_begin);
	kfree((uint64_t *)(reapThis->mm));
	free_page(reapThis->kstack, reapThis->regs.cr3);
	free_file_desc(reapThis);
	delete_page_tables(reapThis->regs.cr3);
	kfree((uint64_t *)reapThis);	
	//TODO: free the memory of the reaped task;	
}

void replace_ptr_in_queue(Task * replace, Task * new_task) {
	if(replace == run_queue) {
		queue_head->next = new_task;
		new_task->next = replace->next;
		run_queue = new_task;
		return;
	}
	Task *curr = run_queue;
	while(curr->next != replace || curr->next != queue_head) {
		curr = curr->next;
	}
	if(curr->next == queue_head ) {
		kprintf("PANIC: replacing something not in queue!\n");
		while(1);
		return;
	}
	curr->next = new_task;
	new_task->next = replace->next;
	return;
}

int pid_exists(uint64_t pid) {

	Task * curr = run_queue;
	while(curr!=queue_head) {
		if(curr->pid == pid && curr->state == READY) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;

}

Task * get_task_from_pid(uint64_t pid) {
	if (pid == 1)
		return queue_head;
	Task * curr = run_queue;
	while(curr!=queue_head) {
		if(curr->pid == pid) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

void reparent_orphans(Task *dyingTask) {
	if(dyingTask==queue_head) {
		kprintf("PANIC: how can pid 1 die?\n");
		while(1);
	}
	Task *curr = run_queue;
	while(curr!=queue_head) {
		if(curr->ppid == dyingTask->pid) {
			curr->ppid = 1;
		}
		curr = curr->next;
	}
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
