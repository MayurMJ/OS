.globl   generic_isr
.align   4
 
generic_isr:
    cli
    pushq    %rax
    pushq    %rcx
    pushq    %rdx
    pushq    %r8
    pushq    %r9
    pushq    %r10
    pushq    %r11
    call    generic_irqhandler
    popq    %rax
    popq    %rcx
    popq    %rdx
    popq    %r8
    popq    %r9
    popq    %r10
    popq    %r11
    movb    $0x20, %al
    outb    %al, $0x20
    sti
    iretq
