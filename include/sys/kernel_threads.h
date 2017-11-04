#ifndef _KERN_THD
#define _KERN_THD

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, rflags, cr3;
} Registers;


typedef struct TASK {
   uint64_t *kstack;
   enum { RUNNING, SLEEPING, ZOMBIE } state;
   Registers regs;
   struct TASK *next;
} Task ;

#endif
