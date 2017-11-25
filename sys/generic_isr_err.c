#include <sys/kmemcpy.h>
#include <sys/paging.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/kernel_threads.h> 
//#include <sys/ahci.h>
void generic_irqhandler_err8(void)
{
    kprintf("Generic interrupt occured with error code 8\n");
	
}

void generic_irqhandler_err10(void)
{
    kprintf("Generic interrupt occured with error code 10\n");

}

void generic_irqhandler_err11(void)
{
    kprintf("Generic interrupt occured with error code 11\n");

}

void generic_irqhandler_err12(void)
{
    kprintf("Generic interrupt occured with error code 12\n");

}

void generic_irqhandler_err13(void)
{
    kprintf("Generic interrupt occured with error code 13\n");

}

void generic_irqhandler_err14(uint64_t errcode)
{
    // TODO: pass registers to it later on
    uint64_t page_fault_addr;
    __asm__ __volatile__("movq %%cr2, %0\n\t"
                             :"=a"(page_fault_addr)); 
    struct mm_struct *curr_mm = CURRENT_TASK->mm;
    struct vma *target_vma = curr_mm->vm_begin;
    while(target_vma != NULL) {
	// scan vmas to see if addr is valid
	uint64_t begin =(uint64_t)(target_vma->vma_start);
	uint64_t end =(uint64_t)(target_vma->vma_end);
	if ((page_fault_addr < end) && (page_fault_addr >= begin)) {
	    break;
	}
	target_vma = target_vma->vma_next;
    }
    if (target_vma == NULL) {
	// seg fault
	kprintf("Unauthorized access!!!\n");
    }
    else {
	// kmemcpy to the right location
	// put page mapping
	// advance file offset- doubt: do we need to always map using offset? i dont think so
		uint64_t cr3val = (uint64_t)(curr_mm->pg_pml4);
		uint64_t aligned_page_fault_addr = ((page_fault_addr>>12)<<12);
		if(errcode & (uint64_t)0x2) {
			// walk PML4 get the physical adress
			uint64_t source = walk_pml4_get_address(aligned_page_fault_addr, cr3val);
			// put_page_mapping
			uint64_t dest = put_page_mapping(7, aligned_page_fault_addr,cr3val);
			source = source + (uint64_t)0xffffffff80000000; 
			source = (source >> 12) << 12;
			// memcpy data from both pages
			kmemcpy((char*)dest, (char*)source, 4096);
		}
		else {	
			put_page_mapping(7,aligned_page_fault_addr,cr3val);
			uint64_t source = (uint64_t)(target_vma->vma_file_ptr)+target_vma->vma_file_offset;
			kmemcpy((char *)aligned_page_fault_addr,(char *)source,4096);
			target_vma->vma_file_offset = target_vma->vma_file_offset + 4096;
		}
    }
}
