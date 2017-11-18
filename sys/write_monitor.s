.globl   write_monitor
.align   4
 
write_monitor:
    cli
    pushq    %rax
    pushq    %rbx
    pushq    %rcx
    pushq    %rdx
    pushq    %r8
    pushq    %r9
    pushq    %r10
    pushq    %r11
    pushq    %r12
    pushq    %r13
    pushq    %r14
    pushq    %r15
    pushq    %rdi
    pushq    %rsi
    pushq    %rbp
    call    write_monitor_irqhandler
    popq    %rbp
    popq    %rsi
    popq    %rdi
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %r11
    popq    %r10
    popq    %r9
    popq    %r8
    popq    %rdx
    popq    %rcx
    popq    %rbx
    popq    %rax
    sti
    iretq
