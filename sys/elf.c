#include <sys/elf64.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kernel_threads.h>
#include <sys/kmalloc.h>
#include <sys/paging.h>
#include <sys/kmemcpy.h>
#include <sys/string.h>
#include <sys/copy_tables.h>
#include <sys/scheduler.h>
// TODO: change this function
uint64_t power (uint64_t x, int e) {
    if (e == 0) return 1;
    return x * power(x, e-1);
}
uint64_t octalToDecimal(uint64_t octal)
{
    uint64_t decimal = 0, i=0;
    while(octal!=0){
        decimal = decimal + (octal % 10) * power(8,i++);
        octal = octal/10;
    }
    return decimal;
}
// TODO: change this function
uint64_t stoi(char *s) // the message and then the line #
{
    uint64_t i;
    i = 0;
    while(*s >= '0' && *s <= '9')
    {
        i = i * 10 + (*s - '0');
        s++;
    }
    return i;
}
// GSAHA: added to test page fault handler
/*void switch_user_mode(uint64_t symbol) {
        __asm__ __volatile__ ( "cli\n\t"
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
                        ::"b"(symbol)
        );
}*/

int strlen(char *s) {
	int len = 0;
	while(s[len] != '\0') {
		len++;
	}
	return (len+1); // includes NULL char at the end
}
uint64_t prep_stack(uint64_t *tos, char* argv[], char *envp[], char *filename) {
	kprintf("binary %s - bfr tos val %p\n",filename,tos);	

	int argc = 1, len; //filename comes first	
	if (argv != NULL) {
		while (argv[argc-1] != NULL) {
			len = strlen(argv[argc-1]);
			tos -= len;
			kmemcpy((char *)tos,argv[argc-1],len);
			argv[argc-1] = (char *)tos;
			argc++;
		}		
	}
	int envp_count = 0;
	if (envp != NULL) {
                while (envp[envp_count] != NULL) {
			len = strlen(envp[envp_count]);
                        tos -= len;
                        kmemcpy((char *)tos,envp[envp_count],len);
                        envp[envp_count] = (char *)tos;
                        envp_count++;
                }
        }
	*tos = '\0'; tos--;
	for (int x=(envp_count-1); x >= 0 ; x--) {
		*tos = (uint64_t)envp[x];
		tos--;
	}
	*tos = '\0'; tos--;
	for (int x= (argc-2); x >= 0; x--) {
		*tos = (uint64_t)argv[x];
		tos--;
	}
	*tos = (uint64_t)filename;
	tos--;
	*tos = argc;
	kprintf("binary %s - aftr tos val %p\n",filename,tos);
	return (uint64_t)tos;
}

Task *loadElf(char *fileName, char *argv[], char *envp[]) { 
	struct posix_header_ustar * header = (struct posix_header_ustar *)&_binary_tarfs_start;
	//kprintf("size of header %d",sizeof(struct posix_header_ustar));
	while(header<(struct posix_header_ustar *)&_binary_tarfs_end) {
		uint64_t size = octalToDecimal(stoi(header->size));
    		if (size == 0) {
			header++;
		}
		else {
			Elf64_Ehdr *elfhdr = (Elf64_Ehdr *) (header+1);
			if((elfhdr->e_ident[0]==0x7f)&&(elfhdr->e_ident[1]==0x45)&&
			(elfhdr->e_ident[2]==0x4c)&&(elfhdr->e_ident[3]==0x46)&& (!strcmp(header->name,fileName))) {
				Task *new_task = (Task*) kmalloc(sizeof(Task));
				new_task->mm = (struct mm_struct *) kmalloc((sizeof(struct mm_struct)));
				new_task->mm->vm_begin = NULL;
				uint8_t *data = (uint8_t *)(header+1);
				Elf64_Phdr *proghdr = (Elf64_Phdr *)&data[elfhdr->e_phoff];
				struct vma* iter;
				int i;
				uint64_t end_addr = 0;
				for(i = 0; i < elfhdr->e_phnum; i++) {
					if(proghdr[i].p_type == ELF_PT_LOAD) {
						struct vma *vm = (struct vma*) kmalloc(sizeof(struct vma));
						vm->vma_start = (uint64_t *)proghdr[i].p_vaddr;
						if((proghdr[i].p_vaddr + proghdr[i].p_memsz) > end_addr) {
							end_addr = (proghdr[i].p_vaddr + proghdr[i].p_memsz);
						}
						vm->vma_end = (uint64_t *)(proghdr[i].p_vaddr + proghdr[i].p_memsz);
						vm->vma_file_ptr = (uint64_t *)(&data[proghdr[i].p_offset]); 
						// GSAHA: added to test page fault handle
						vm->vma_file_offset = 0;
						vm->vma_size = proghdr[i].p_filesz;
						vm->vma_mem_size = proghdr[i].p_memsz;
						vm->vma_flags = proghdr[i].p_flags;
						vm->vma_next = NULL;
						if(new_task->mm->vm_begin == NULL) {
							new_task->mm->vm_begin = vm;
						}
						else {
							for(iter = new_task->mm->vm_begin; iter->vma_next != NULL; iter = iter->vma_next);
							iter->vma_next = vm;
						} 
					}
				}
				end_addr += 4096;
				end_addr = (end_addr >> 12) << 12;
				// Go to the end of the Vma list and add vma for heap
				for(iter = new_task->mm->vm_begin; iter->vma_next != NULL; iter = iter->vma_next);
				struct vma *vm = (struct vma*) kmalloc(sizeof(struct vma));
				vm->vma_start = (uint64_t *) end_addr;
				vm->vma_end = (uint64_t *) end_addr;
				vm->vma_next = NULL;
				iter->vma_next = vm;
				// Add vma entry for stack
				struct vma *vm_stack = (struct vma*) kmalloc(sizeof(struct vma));
				vm_stack->vma_start = (uint64_t *) USER_STACK - USER_STACK_SIZE;
				vm_stack->vma_end = (uint64_t *) USER_STACK;
				vm_stack->vma_next = NULL;
				vm->vma_next = vm_stack;
				// Allocate 1 page for stack for now and add it to the new cr3 page mapping
				uint64_t newcr3 = create_table(); //preps the PMl4 table only
				uint64_t oldcr3;
				__asm__ __volatile__("movq %%cr3, %0\n\t"
						    :"=a"(oldcr3));
				__asm__ __volatile__("movq %0, %%cr3\n\t"
						    ::"a"(newcr3));
				new_task->mm->pg_pml4=newcr3;
				new_task->next = NULL;
				new_task->prev = NULL;
				new_task->children = NULL;
				new_task->sibling = NULL;
				new_task->regs.cr3 = newcr3;
				put_page_mapping(USER_ACCESSIBLE,USER_STACK, newcr3);
				put_page_mapping(USER_ACCESSIBLE,USER_STACK - 4096, newcr3);
				new_task->mm->stack_begin = (uint64_t) (USER_STACK);
				// prep stack
				uint64_t tos = prep_stack((uint64_t *)(new_task->mm->stack_begin), argv, envp, fileName);
				kprintf("tos val %x\n",tos);
				new_task->mm->stack_begin = tos;
				// Allocating a dummy file obj for stdin so its not null
				new_task->file_desc[0] = kmalloc(sizeof(struct FILE_OBJ));
				/*
				new_task->file_desc[0]->file_begin = USER_READ_BUFFER;
				new_task->file_desc[0]->file_end = USER_READ_BUFFER+4095;
				new_task->file_desc[0]->file_offset = 0;
				*/
				int x;
				for(x=1; x< MAX_FDS;x++)
					new_task->file_desc[x] = NULL;
                                // may move above part
				__asm__ __volatile__("movq %0, %%cr3\n\t"
						    ::"a"(oldcr3));        
				// might need to change the part above
				
				new_task->mm->e_entry = elfhdr->e_entry;
				return new_task;
			}
			size = (size%512==0) ? size +512: size + 512 + (512-size%512);
			header = (struct posix_header_ustar *) (((uint64_t)(header)) + size);
		}
	}
	return NULL; 
}


