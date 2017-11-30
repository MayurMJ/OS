#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kmalloc.h>
#include <sys/paging.h>
#include <sys/kernel_threads.h>
#include <sys/copy_tables.h>
#include <sys/elf64.h>
#include <sys/scheduler.h>

/*TODO populate syscall number
void * syscall_tbl[NUM_SYSCALLS] = 
{
	sys_fork,
	opendir,
}
*/
uint64_t yyield() {
    Task *last = CURRENT_TASK;
    CURRENT_TASK = CURRENT_TASK->next;
    switchTaskUser(&last->regs, &CURRENT_TASK->regs);
    return CURRENT_TASK->pid;
}
void deep_copy_vma(struct vma *parent, struct vma *child) {

    child->vma_start = parent->vma_start;
    child->vma_end = parent->vma_end;
    child->vma_file_ptr = parent->vma_file_ptr;
    child->vma_file_offset = parent->vma_file_offset;
    child->vma_flags = parent->vma_flags;
    child->vma_size = parent->vma_size;
    
}

void copy_vma_list(struct vma *parent,struct mm_struct *child) {

    struct vma * curr_par_vma = parent;
    
    if(parent) {
        child->vm_begin = (struct vma *) kmalloc(sizeof(struct vma ));
	deep_copy_vma(parent,child->vm_begin);
        child->vm_begin->vma_next =NULL;
    }
    curr_par_vma = parent->vma_next;
    struct vma * child_vma = child->vm_begin; 
    
    while(curr_par_vma) {
	struct vma * tempvma = (struct vma *)kmalloc(sizeof(struct vma ));
	deep_copy_vma(curr_par_vma,tempvma);
	child_vma->vma_next = tempvma;
	child_vma = tempvma;
	curr_par_vma = curr_par_vma->vma_next;
    }
}
void duplicate_fds(Task *parent, Task *child) {
    int i;
    for (i=0; i<MAX_FDS; i++)
	child->file_desc[i] = parent->file_desc[i];
    return;
}
void copy_to_child(Task *parent_task, Task *child_task) {

    child_task->ppid = parent_task->pid;
    child_task->pid =  (last_assn_pid+1)%MAX_PROC ;
    last_assn_pid++;
    child_task->state = WAITING;
    duplicate_fds(parent_task, child_task); 
    //copy mm_struct
    child_task->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));
    child_task->mm->stack_begin = parent_task->mm->stack_begin;
    child_task->mm->e_entry = parent_task->mm->e_entry;
    child_task->mm->pg_pml4 = copy_on_write();
    child_task->kstack = (uint64_t *)( (uint64_t)get_free_page(SUPERVISOR_ONLY, child_task->mm->pg_pml4));
    child_task->regs.cr3 = child_task->mm->pg_pml4;
    copy_vma_list(parent_task->mm->vm_begin, child_task->mm);
    // TODO: need to change this circular mapping
     put_in_run_queue(child_task);

}

//#include <sys/ahci.h>
uint64_t fork_handler(Task * child_task) {
    Task * parent_task = CURRENT_TASK;

//    child_task->regs = *reg;
    copy_to_child(parent_task,child_task);
    return child_task->pid;
    //TODO To be continued ....

}    

ssize_t read_handler(int fd, char *buf, size_t count) {
    // TODO: not checking bounds on buf
    if (fd == 0) { // special treatment for stdin
	// if no chars are available just return 0 right away
	if (TERM_BUF_OFFSET == 0)
		return 0;
	int x, chars_read =0; // whats the use of file offset in this case??
	// if lesser num of bytes are available than count then read only available number of chars
	if (TERM_BUF_OFFSET < count)
		count = TERM_BUF_OFFSET; 
	
	for (x=0; x < count;x++) {
		char c = *(char *)(TERMINAL_BUFFER+TERM_BUF_OFFSET);
		if (c == '\n') {
			chars_read++;
			*buf = c;
			return chars_read;
		}
		*buf = c;
		buf++;
		chars_read++;
	}
	return -1;
    }
    else { // read regular files
    }
    return -1;
}

uint64_t syscall_handler_old(void)
{
    // don't put anything before this!!!
    uint64_t arg1,arg2,arg3,arg4;
    __asm__ __volatile__("movq %%rdi, %0\n\t"
			 "movq %%rsi, %1\n\t"
			 "movq %%rdx, %2\n\t"
			 "movq %%rcx, %3\n\t"
			 :"=D"(arg1),"=S"(arg2),"=d"(arg3),"=c"(arg4));
    /*
    uint64_t *binaryname=NULL;
    __asm__ __volatile__("movq %%rdi, %0\n\t"
                         :"=D" (binaryname) :);
     */
    uint64_t rsp;
    __asm__ __volatile__("movq %%rbx, %0\n\t"
                        :"=b" (rsp)
                        :);
    uint64_t syscall_number=0;
    __asm__ __volatile__("movq %%rax, %0\n\t"
			:"=a" (syscall_number)
                        :);
    kprintf("Syscallno %d from process %d\n",syscall_number,CURRENT_TASK->pid);
    switch(syscall_number) {
	case 0:; /* read syscall-arg1-file desc, arg2-buffer to copy to, arg3-number of chars to copy or till \n*/
		/*
		if (fd == 0) {
			while (1) {
				if (FG_TASK != NULL) {
					// save state and schedule another task
				}
				else
					break;
			}
			FG_TASK = CURRENT_TASK;
			int fd = arg1;
			char *buffer = (char *)arg2;
			int count = arg3;
			ssize_t chars_read = read_handler(fd, buffer, count);
			if (chars_read == -1) { //input is not ready yet, should I take it at 0 available?
				FG_TASK->state = WAITING;
				// save state and schedule next task	
			}
			// adjust terminal buffer and offset and unset fg task
			// TODO: what would be a better startegy? - should i just empty buffer here or keep the buffer content?
			int x;
			for (x= chars_read; x <4096; x++) {
				*(char *)(TERMINAL_BUFFER + x - chars_read) = *(char *)(TERMINAL_BUFFER + x);
			}
			TERM_BUF_OFFSET -= chars_read;
			FG_TASK = NULL;
			return chars_read;
		}
		*/
		break;
	case 10:
    	        kprintf("I'm in parent process %d\n",CURRENT_TASK->pid);
		break;
	case 9:
		kprintf("Hi from sbush\n");
		break;
	case 11:
    	        kprintf("I'm in child process %d with arc %d\n",CURRENT_TASK->pid, arg1);
		schedule();
		break;
	case 24:
	        schedule();
		return 1000;
		break;
	case 57:;
		//kprintf("rsp value %x\n",rsp);
    		Task * child_task = (Task *) kmalloc(sizeof(Task));
                Registers *reg = (Registers *)&child_task->regs;
		__asm__ __volatile__("movq %%ds, %0\n\t"
                      		    :"=a" (reg->ds)
                        	    :);
                __asm__ __volatile__("movq %%es, %0\n\t"
                      		    :"=a" (reg->es)
                        	    :);
                __asm__ __volatile__("movq %%fs, %0\n\t"
                      		    :"=a" (reg->fs)
                        	    :);
                __asm__ __volatile__("movq %%gs, %0\n\t"
                      		    :"=a" (reg->gs)
                        	    :);
	
		kprintf("rsp value %x\n",rsp);
		uint64_t ret = fork_handler(child_task);
	        saveState(reg);
		return ret;
		break;
	case 59:; /* execve- rdi-binary name,rsi-argv,rdx-envp*/
		//kprintf("%d %d\n",arg2, arg3);	
		//kprintf("%s\n",arg1);
		char **argv; char **envp;
		argv = (char **)arg2;
		envp = (char **)arg3;
		Task *replacement_task = loadElf((char *)arg1, argv, envp);
		replacement_task->regs.rax = 0;
    		replacement_task->regs.rbx = 0;
    		replacement_task->regs.rcx = 0;
    		replacement_task->regs.rdx = 0;
    		replacement_task->regs.rsi = 0;
    		replacement_task->regs.rdi = 0;
    		replacement_task->regs.rflags = CURRENT_TASK->regs.rflags;
    		replacement_task->regs.rip = replacement_task->mm->e_entry;
   		replacement_task->regs.r8 = 0;
    		replacement_task->regs.r9 = 0;
    		replacement_task->regs.r10 = 0;
    		replacement_task->regs.r11 = 0;
    		replacement_task->regs.r12 = 0;
    		replacement_task->regs.r13 = 0;
    		replacement_task->regs.r14 = 0;
    		replacement_task->regs.r15 = 0;
    		replacement_task->regs.rsp = replacement_task->mm->stack_begin;
    		replacement_task->regs.ds = CURRENT_TASK->regs.ds;
		replacement_task->regs.es = CURRENT_TASK->regs.es;
		replacement_task->regs.fs = CURRENT_TASK->regs.fs;
		replacement_task->regs.gs = CURRENT_TASK->regs.gs;
		replacement_task->regs.ss = CURRENT_TASK->regs.ss;
		replacement_task->regs.cs = CURRENT_TASK->regs.cs;
		// put in run queue and give it the same pid
		replacement_task->pid = CURRENT_TASK->pid;
		// TODO: remove this circular list
		replacement_task->next = CURRENT_TASK->next;
		replacement_task->prev = CURRENT_TASK->prev;
		replacement_task->next->next = replacement_task;
		replacement_task->next->prev = replacement_task;
		
		CURRENT_TASK->next = replacement_task;
		delete_page_tables(CURRENT_TASK->regs.cr3);
		yyield();	
		return -1; // if execve returns its an error
		break;
	case 60: /* exit- rdi-return value of main*/
//		if((uint64_t)arg1 == 0) {//main returned 0, normal exit
//			Task * deleteme = CURRENT_TASK;
//			
//		}
		kprintf("exited with %d\n", (uint64_t)arg1);
		remove_from_run_queue(CURRENT_TASK);
		schedule();
		break;
	default:
		kprintf("Syscall not found \n");
    }
	return 1;
}

void syscall_handler(void)
{
    // don't put anything before this!!!
    uint64_t arg1,arg2,arg3,arg4;
    __asm__ __volatile__("movq %%rdi, %0\n\t"
			 "movq %%rsi, %1\n\t"
			 "movq %%rdx, %2\n\t"
			 "movq %%rcx, %3\n\t"
			 :"=D"(arg1),"=S"(arg2),"=d"(arg3),"=c"(arg4));
    /*
    uint64_t *binaryname=NULL;
    __asm__ __volatile__("movq %%rdi, %0\n\t"
                         :"=D" (binaryname) :);
     */
    uint64_t syscall_number=0;
    __asm__ __volatile__("movq %%rax, %0\n\t"
			:"=a" (syscall_number)
                        :);
    kprintf("Syscallno %d from process %d\n",syscall_number,CURRENT_TASK->pid);
    switch(syscall_number) {
	case 0:; /* read syscall-arg1-file desc, arg2-buffer to copy to, arg3-number of chars to copy or till \n*/
		/*
		if (fd == 0) {
			while (1) {
				if (FG_TASK != NULL) {
					// save state and schedule another task
				}
				else
					break;
			}
			FG_TASK = CURRENT_TASK;
			int fd = arg1;
			char *buffer = (char *)arg2;
			int count = arg3;
			ssize_t chars_read = read_handler(fd, buffer, count);
			if (chars_read == -1) { //input is not ready yet, should I take it at 0 available?
				FG_TASK->state = WAITING;
				// save state and schedule next task	
			}
			// adjust terminal buffer and offset and unset fg task
			// TODO: what would be a better startegy? - should i just empty buffer here or keep the buffer content?
			int x;
			for (x= chars_read; x <4096; x++) {
				*(char *)(TERMINAL_BUFFER + x - chars_read) = *(char *)(TERMINAL_BUFFER + x);
			}
			TERM_BUF_OFFSET -= chars_read;
			FG_TASK = NULL;
			return chars_read;
		}
		*/
		break;
	case 10:
    	        kprintf("I'm in parent process %d\n",CURRENT_TASK->pid);
		break;
	case 9:
		kprintf("Hi from sbush\n");
		break;
	case 11:
    	        kprintf("I'm in child process %d with arc %d\n",CURRENT_TASK->pid, arg1);
		schedule();
		break;
	case 24:
	        schedule();
		break;
	case 57:;
		//kprintf("rsp value %x\n",rsp);
    		Task * child_task = (Task *) kmalloc(sizeof(Task));
                Registers *reg = (Registers *)&child_task->regs;
		__asm__ __volatile__("movq %%ds, %0\n\t"
                      		    :"=a" (reg->ds)
                        	    :);
                __asm__ __volatile__("movq %%es, %0\n\t"
                      		    :"=a" (reg->es)
                        	    :);
                __asm__ __volatile__("movq %%fs, %0\n\t"
                      		    :"=a" (reg->fs)
                        	    :);
                __asm__ __volatile__("movq %%gs, %0\n\t"
                      		    :"=a" (reg->gs)
                        	    :);
	
		uint64_t ret = fork_handler(child_task);
                __asm__ __volatile__("movq %0, %%rax\n\t"
                                    :"=a" (ret)
                                    :);
	        saveState(reg);
		break;
	case 59:; /* execve- rdi-binary name,rsi-argv,rdx-envp*/
		//kprintf("%d %d\n",arg2, arg3);	
		//kprintf("%s\n",arg1);
		char **argv; char **envp;
		argv = (char **)arg2;
		envp = (char **)arg3;
		Task *replacement_task = loadElf((char *)arg1, argv, envp);
		replacement_task->regs.rax = 0;
    		replacement_task->regs.rbx = 0;
    		replacement_task->regs.rcx = 0;
    		replacement_task->regs.rdx = 0;
    		replacement_task->regs.rsi = 0;
    		replacement_task->regs.rdi = 0;
    		replacement_task->regs.rflags = CURRENT_TASK->regs.rflags;
    		replacement_task->regs.rip = replacement_task->mm->e_entry;
   		replacement_task->regs.r8 = 0;
    		replacement_task->regs.r9 = 0;
    		replacement_task->regs.r10 = 0;
    		replacement_task->regs.r11 = 0;
    		replacement_task->regs.r12 = 0;
    		replacement_task->regs.r13 = 0;
    		replacement_task->regs.r14 = 0;
    		replacement_task->regs.r15 = 0;
    		replacement_task->regs.rsp = replacement_task->mm->stack_begin;
    		replacement_task->regs.ds = CURRENT_TASK->regs.ds;
		replacement_task->regs.es = CURRENT_TASK->regs.es;
		replacement_task->regs.fs = CURRENT_TASK->regs.fs;
		replacement_task->regs.gs = CURRENT_TASK->regs.gs;
		replacement_task->regs.ss = CURRENT_TASK->regs.ss;
		replacement_task->regs.cs = CURRENT_TASK->regs.cs;
		// put in run queue and give it the same pid
		replacement_task->pid = CURRENT_TASK->pid;
		// TODO: remove this circular list
		replacement_task->next = CURRENT_TASK->next;
		replacement_task->prev = CURRENT_TASK->prev;
		replacement_task->next->next = replacement_task;
		replacement_task->next->prev = replacement_task;
		
		CURRENT_TASK->next = replacement_task;
		delete_page_tables(CURRENT_TASK->regs.cr3);
		yyield();	
		break;
	case 60: /* exit- rdi-return value of main*/
//		if((uint64_t)arg1 == 0) {//main returned 0, normal exit
//			Task * deleteme = CURRENT_TASK;
//			
//		}
		kprintf("exited with %d\n", (uint64_t)arg1);
		remove_from_run_queue(CURRENT_TASK);
		schedule();
		break;
	default:
		kprintf("Syscall not found \n");
    }
	__asm__ __volatile__("iretq;");
}
