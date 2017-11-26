.globl   syscall
.align   4
 
syscall:
    cli
    pushq    %rbx  # Hack to ignore rax
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
    movq     %rsp, %rcx
    call    syscall_handler
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
    popq    %rbx
    sti
    iretq
