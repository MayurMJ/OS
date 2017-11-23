#ifndef _KERN_THD
#define _KERN_THD
#include <sys/defs.h>

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rflags, cr3;
} Registers;

struct mm_struct {
    //uint64_t code_begin, code_end, data_begin, data_end, stack_begin, brk_start;
    struct vma *vm_begin;
    uint64_t *pg_pml4;	
};

struct vma {
    struct mm_struct *vma_mm;
    uint64_t *vma_start;
    uint64_t *vma_end;
    uint64_t *vma_file_ptr;
    uint64_t vma_file_offset;
    uint64_t vma_flags;
    uint64_t vma_size;
    struct vma *vma_prev;
    struct vma *vma_next;
};

typedef struct TASK {
   uint64_t *kstack;
   enum { RUNNING, SLEEPING, ZOMBIE } state;
   Registers regs;
   struct TASK *next;
   struct mm_struct *mm;
} Task ;

extern void switchTask(Registers *oldregs, Registers *newregs); 
#endif
