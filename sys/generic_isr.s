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
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call    generic_irqhandler
    popq    %rax
    popq    %rcx
    popq    %rdx
    popq    %r8
    popq    %r9
    popq    %r10
    popq    %r11
    sti
    movb    $0x20, %al
    outb    %al, $0x20
    iret
