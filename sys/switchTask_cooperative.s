.section .text
.global switchTask
# rax 0, rbx 8, rcx 16, rdx 24, rsi 32, rdi 40, rsp 48, rbp 56, rip 64, rflags 72, cr3 80
# r8 88, r9 96, r10 104, r11 112, r12 120, r13 128, r14 136, r15 144
# ------------------push order----------------
# 160			  "pushq %rax\n\t"
# 152                        "pushq %rbx\n\t"
# 144                        "pushq %rcx\n\t"
# 136                        "pushq %rdx\n\t"
# 128                        "pushq %rdi\n\t"
# 120                        "pushq %rsi\n\t"
# 112                        "pushq %rbp\n\t"
# 104                        "pushq %r8\n\t"
# 96                        "pushq %r9\n\t"
# 88                      "pushq %r10\n\t"
# 80                        "pushq %r11\n\t"
# 72                        "pushq %r12\n\t"
# 64                        "pushq %r13\n\t"
# 56                        "pushq %r14\n\t"
# 48                        "pushq %r15\n\t"

switchTask:
	pushq %rax # +136
        movq %rsp, %rax
        addq $16, %rax # 8 for rax and 8 for the return addr so the top of the stack is 2 words away
        push %rax #pushing %RSP + 128
        pushq %rbx #pushing %RBX + 120
        pushq %rcx # pushing RCX + 112
        pushq %rdx # pushing RDX + 104
        pushq %rdi # pushing RDI + 96
        pushq %rsi # pushing RSI + 88
        pushq %rbp # pushing RBP + 80
        pushq %r8 # pushing r8 + 72
        pushq %r9 # pushing r9 + 64
        pushq %r10 #pushing r10 + 56
        pushq %r11 # pushing r11 + 48
        pushq %r12 #pushing r12 + 40
        pushq %r13 # pushing r13 + 32
        pushq %r14 #pushing r 14 + 24
        pushq %r15 #pushing r15 at +16
        pushfq # pushing flags  + 8
        movq %cr3, %rax
        pushq %rax #pushing cr3 at rsp

        movq %rdi, %rax # getting the current register structure and deferencing rax gives the location of the register info
        movq %rbx, 24(%rax) # moving the value of rbx to correct location checked
        movq %rcx, 32(%rax) # moving rcx to correct location checked
        movq %rdx, 40(%rax) # moving rdx to correct location checked
	movq %rdi, 48(%rax) # moving rdi to correct location checked
        movq %rsi, 56(%rax) # moving rcx to correct location checked
        movq %rbp, 64(%rax) # moving rbp to correct location checked
        movq %r8, 72(%rax) # moving r8 to correct location checked
        movq %r9, 80(%rax) # moving r9 to correct location checked
        movq %r10, 88(%rax) # moving r10 to correct location checked
        movq %r11, 96(%rax) # moving r11 to correct location checked
        movq %r12, 104(%rax) # moving r12 to correct location checked
        movq %r13, 112(%rax) # moving r13 to correct location checked
        movq %r14, 120(%rax) # moving r14 to correct location checked
        movq %r15, 128(%rax) # moving r15 to correct location checked
        movq 136(%rsp), %rbp #using rbp to store rax since we have already stored rbp
        movq 144(%rsp), %rcx #using rcx to store rip rip is the return address that is stored on the stack when the interrupt handler is called
        movq 128(%rsp), %rdx #using rdx to store rsp that was calculated and storing it in rdx.
        movq 8(%rsp), %rdi # rdi contains flags that can be used
        movq %rbx, 16(%rax) # storing rax in he right location in register ( current process state)
        movq %rdx, (%rax) # storing rsp in the correct location in registers
        movq %rcx, 8(%rax) # rip is moved to the correct location in the registers
        movq %rdi, 144(%rax) # flags are stored in the correct location in the registers

        popq %rbx # this is getting the cr3 we stored
        movq %rbx, 136(%rax) #cr3 is stored in the correct location
        pushq %rbx # putting cr3 back where it belongs

        movq %rsi, %rax # preparing for the new register values, the new register address is passed in rsi
        movq 24(%rax), %rbx #
	movq 32(%rax), %rcx #
        movq 40(%rax), %rdx #
        movq 48(%rax), %rdi #
        movq 56(%rax), %rsi #
        movq 64(%rax), %rbp #
        movq 72(%rax), %r8 #
        movq 80(%rax), %r9 #
        movq 88(%rax), %r10 #
        movq 96(%rax), %r11 #
        movq 104(%rax), %r12 #
        movq 112(%rax), %r13 #
        movq 120(%rax), %r14 #
        movq 128(%rax), %r15 #

        pushq %rax # storing rax we going to still use it but we want to use rax for storing flags
        movq 144(%rax), %rax # moving flags to rax, destroying rax in the process :p OMG! but we stored it in rax mwahaha, not so easily sucker!
        pushq %rax # we cannot move to flags to push and pop to flags
        popfq # updating flags

        popq %rax # ahh, rax is back to the new registers, all is well! "smart stuff this is" - yoda
        movq (%rax), %rsp # rsp is updated from the new registers
        pushq %rax # pushing rax again to store it
        movq 136(%rax), %rax # the big stuff the cr3 is loaded into rax
        movq %rax, %cr3 # and boom page tables changed
        popq %rax # get back our new register address again
        pushq %rax
	movq 8(%rax), %rax #
        xchgq (%rsp), %rax # this is how this works, you pushed rax into stack now deferencing rsp gives you rax and rax has rip so it swaps that stack top contains rip that what we want coz on retq the stack top is popped
        movq 16(%rax), %rax # now finally rax is what it is supposed to be
        retq
