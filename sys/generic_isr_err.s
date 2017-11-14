.macro	 set_generic_isr_err    index
.globl   generic_isr_err\index
.align   4
generic_isr_err\index:
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
    call    generic_irqhandler_err\index
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
    addq    $8, %rsp # pop error code that was pushed in
    sti
    iretq
.endm

set_generic_isr_err 8
set_generic_isr_err 10
set_generic_isr_err 11
set_generic_isr_err 12
set_generic_isr_err 13
set_generic_isr_err 14
