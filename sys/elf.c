#include <sys/elf64.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kernel_threads.h>
#include <sys/kmalloc.h>
#include <sys/paging.h>
#include <sys/kmemcpy.h>
#include <sys/string.h>
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
void switch_user_mode(uint64_t symbol) {
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
}

void loadElf(char *fileName) { 
  
  struct posix_header_ustar * header = (struct posix_header_ustar *)&_binary_tarfs_start;
  kprintf("size of header %d",sizeof(struct posix_header_ustar));
  while(header<(struct posix_header_ustar *)&_binary_tarfs_end) {
    uint64_t size = octalToDecimal(stoi(header->size));
    if (size == 0)
      header++;
    else {
      Elf64_Ehdr *elfhdr = (Elf64_Ehdr *) (header+1);
      if((elfhdr->e_ident[0]==0x7f)&&(elfhdr->e_ident[1]==0x45)&&
        (elfhdr->e_ident[2]==0x4c)&&(elfhdr->e_ident[3]==0x46)&& (!strcmp(header->name,fileName))) {

        Task *t = (Task*) kmalloc(sizeof(Task));
        t->mm = (struct mm_struct *) kmalloc((sizeof(struct mm_struct)));
        t->mm->vm_begin = NULL;
        uint8_t *data = (uint8_t *)(header+1);
        Elf64_Phdr *proghdr = (Elf64_Phdr *)&data[elfhdr->e_phoff];
        struct vma* iter;
        int i;
        uint64_t end_addr = 0;
        for(i=0;i<elfhdr->e_phnum;i++) {

          if(proghdr[i].p_type == ELF_PT_LOAD) {
            kprintf("need to load this\n");
            uint64_t cr3val;
            __asm__ __volatile__("movq %%cr3, %0\n\t"
                             :"=a"(cr3val));
            put_page_mapping(7,proghdr->p_vaddr,cr3val);
            struct vma *vm = (struct vma*) kmalloc(sizeof(struct vma));
            vm->vma_start = (uint64_t *)proghdr->p_vaddr;
            if((proghdr->p_vaddr + proghdr->p_memsz) > end_addr) {
              end_addr = (proghdr->p_vaddr + proghdr->p_memsz);
            }
            vm->vma_end = (uint64_t *)(proghdr->p_vaddr + proghdr->p_memsz);
            vm->vma_file_ptr = (uint64_t *)(&data[proghdr->p_offset]); 
            vm->vma_size = proghdr->p_filesz;
            vm->vma_flags = proghdr->p_flags;
            vm->vma_next = NULL;
            if(t->mm->vm_begin == NULL) t->mm->vm_begin = vm;
            else {
             for(iter = t->mm->vm_begin; iter->vma_next != NULL; iter = iter->vma_next);
             iter->vma_next = vm;
            } 
          //kmemcpy((char *)proghdr->p_vaddr,(char *)&data[proghdr->p_offset],proghdr->p_memsz);
	 //kprintf("%x %x %x %x\n",proghdr->p_vaddr,proghdr->p_paddr,proghdr->p_filesz,proghdr->p_memsz);
          }
	 //switch_user_mode(elfhdr->e_entry);
        }
        
      }
      size = (size%512==0) ? size +512: size + 512 + (512-size%512);
      header = (struct posix_header_ustar *) (((uint64_t)(header)) + size);
    }  
  }  
}
