#ifndef _KERN_THD
#define _KERN_THD

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rflags, cr3;
} Registers;


typedef struct TASK {
   uint64_t *kstack;
   enum { RUNNING, SLEEPING, ZOMBIE } state;
   Registers regs;
   struct TASK *next;
} Task ;

extern void switchTask(Registers *oldregs, Registers *newregs); 
#endif
