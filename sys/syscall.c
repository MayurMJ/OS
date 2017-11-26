#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kmalloc.h>
#include <sys/paging.h>
#include <sys/kernel_threads.h>
#include <sys/copy_tables.h>

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

void copy_to_child(Task *parent_task, Task *child_task) {

    child_task->ppid = parent_task->pid;
    child_task->pid = 10 ;//TODO assigning random numner as of now, implement get_new_pid();
    child_task->kstack = (uint64_t *)( (uint64_t)get_free_page(7) + 4080);
    child_task->state = WAITING;
    
    //copy mm_struct
    child_task->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));
    child_task->mm->stack_begin = parent_task->mm->stack_begin;
    child_task->mm->e_entry = parent_task->mm->e_entry;
    child_task->mm->pg_pml4 = copy_on_write();
    child_task->regs.cr3 = child_task->mm->pg_pml4;
    copy_vma_list(parent_task->mm->vm_begin, child_task->mm);

    child_task->next = parent_task;
    parent_task->next = child_task;

}

//#include <sys/ahci.h>
uint64_t fork_handler(Registers *reg) {
    Task * child_task = (Task *) kmalloc(sizeof(Task));
    Task * parent_task = CURRENT_TASK;

    child_task->regs = *reg;
    copy_to_child(parent_task,child_task);
    return child_task->pid;
    //TODO To be continued ....

}    
uint64_t syscall_handler(void)
{
    uint64_t rsp;
    __asm__ __volatile__("movq %%rcx, %0\n\t"
                        :"=c" (rsp)
                        :);
    uint64_t syscall_number=0;
    __asm__ __volatile__("movq %%rax, %0\n\t"
			:"=a" (syscall_number)
                        :);
    kprintf("Syscallno %d from process %d\n",syscall_number,CURRENT_TASK->pid);
    switch(syscall_number) {
	case 10:
    	        kprintf("I'm in parent process %d\n",CURRENT_TASK->pid);
		break;
	case 11:
    	        kprintf("I'm in child process %d\n",CURRENT_TASK->pid);
		break;
	case 24:
	        return	yyield();
		break;
	case 57:;
		//kprintf("rsp value %x\n",rsp);
                Registers *reg = (Registers*) kmalloc(sizeof(Registers));
	        saveState(reg, rsp);
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
 	
		uint64_t ret = fork_handler(reg);
		kprintf("rsp value %x\n",rsp);
		return ret;
		break;
	case 59: /* execve- rdi-binary name,rsi-argv,rdx-envp*/
		break;
	case 60: /* exit- rdi-return value of main*/
		break;
	default:
		kprintf("Syscall not found \n");
    }
	return 57;
}
