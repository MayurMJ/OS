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
    child_task->kstack = (uint64_t *)( (uint64_t)get_free_page(7) + 4088);
    child_task->state = WAITING;
    
    //copy mm_struct
    child_task->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));
    child_task->mm->stack_begin = parent_task->mm->stack_begin;
    child_task->mm->e_entry = parent_task->mm->e_entry;
    child_task->mm->pg_pml4 = copy_on_write();

    copy_vma_list(parent_task->mm->vm_begin, child_task->mm);

    child_task->next = parent_task;
    parent_task->next = child_task;

}

//#include <sys/ahci.h>
void fork_handler() {
    Task * child_task = (Task *) kmalloc(sizeof(Task));
    Task * parent_task = CURRENT_TASK;

    copy_to_child(parent_task,child_task);

    //TODO To be continued ....

}    
void syscall_handler(void)
{
    uint64_t rsp = 0; 
    __asm__ __volatile__("movq %%rsp, %0\n\t"
			:"=b" (rsp)
                        :);
    rsp = rsp - 16; 
    uint64_t syscall_number=0;
    __asm__ __volatile__("movq %%rax, %0\n\t"
			:"=a" (syscall_number)
                        :);
    switch(syscall_number) {
	case 57:;
                Registers reg;
	        saveState(&reg, rsp); 	
		//fork_handler();
		break;
	default:
		kprintf("Syscall not found \n");
    }
    kprintf("Printing happening %d\n",syscall_number);
	
}
