.section .text
.global saveState
saveState:
        pushq   %rax
        pushq   %rbx
        pushq   %rcx
        pushq   %rdx
        pushq   %rsi
        pushq   %rdi
        movq   %rsp, %rax
        pushq   %rax
        pushq   %rbp
        pushq   $5  # pushing rip directly gives an issue, so pushing a dummy value as placeholder
        pushq   %r8
        pushq   %r9
        pushq   %r10
        pushq   %r11
        pushq   %r12
        pushq   %r13
        pushq   %r14
        pushq   %r15
        pushfq
        movq    %cr3, %rax
        pushq   %rax # pushing stuff is over
        movq    %rdi, %rax # from is copied to rax
        movq    %rbx, 8(%rax)
        movq    %rcx, 16(%rax)
        movq    %rdx, 24(%rax)
        movq    %rsi, 32(%rax) # can't just copy rsi and rdi they have been overwritten, not sure though
        movq    %rdi, 40(%rax) # muthu didn't do it, movq 160(%rsp),32(%rax) and movq 168(%rsp),40(%rax)
        movq    %r8, 72(%rax)
        movq    %r9, 80(%rax)
        movq    %r10, 88(%rax)
        movq    %r11, 96(%rax)
        movq    %r12, 104(%rax)
        movq    %r13, 112(%rax)
        movq    %r14, 120(%rax)
        movq    %r15, 128(%rax) # copied the regular regs
        movq    144(%rsp), %r9 # rax
        movq    152(%rsp), %rcx # ip , changed it to access the ret addr pushed into stack bfr switch call
        movq    96(%rsp), %rdx # rsp
        addq    $56, %rdx # might need to pop beyond the return addr, not sure
        movq    88(%rsp), %r8 # rbp
        movq    8(%rsp), %rdi # eflags
        movq    $0, (%rax)  # rax
        movq    %rdx, 48(%rax) # rsp
        movq    %r8, 56(%rax) # rbp
        movq    %rcx, 64(%rax) # ip
        movq    %rdi, 136(%rax) # eflags
        popq    %r9 #cr3
        movq    %r9, 144(%rax)
        pushq   %r9
	movq	%rsp, %r10
	addq	$152, %r10
	movq	%r10, %rsp
	retq
/*
	movq	%rdi, %rbx # Register Pointer where we need to store stuff
	movq    %rsi, %rcx # rsp value of the interrupt Handler before calling syscall handler

	movq	(%rcx), %rax
	movq	%rax, 56(%rbx)  #rbp
	
	movq	8(%rcx), %rax
	movq	%rax, 32(%rbx)  #rsi
	
	movq	16(%rcx), %rax
	movq	%rax, 40(%rbx)  #rdi
	
	movq	24(%rcx), %rax
	movq	%rax, 128(%rbx)  #r15
	
	movq	32(%rcx), %rax
	movq	%rax, 120(%rbx)  #r14
	
	movq	40(%rcx), %rax
	movq	%rax, 112(%rbx)  #r13
	
	movq	48(%rcx), %rax
	movq	%rax, 104(%rbx)  #r12

	movq	56(%rcx), %rax
	movq	%rax, 96(%rbx)  #r11
	
	movq	64(%rcx), %rax
	movq	%rax, 88(%rbx)  #r10
	
	movq	72(%rcx), %rax
	movq	%rax, 80(%rbx)  #r9
	
	movq	80(%rcx), %rax
	movq	%rax, 72(%rbx)  #r8
	
	movq	88(%rcx), %rax
	movq	%rax, 24(%rbx)  #rdx
	
	movq	96(%rcx), %rax
	movq	%rax, 16(%rbx)  #rcx
	
	movq	104(%rcx), %rax
	movq	%rax, 8(%rbx)  #rbx
	
	movq	112(%rcx), %rax
	movq	$0, (%rbx)  #rax hard code rax to 0 as child fork returns 0
	
	movq	120(%rcx), %rax
	movq	%rax, 64(%rbx)  #rip
	
	movq	128(%rcx), %rax
	movq	%rax, 152(%rbx)  #cs
	
	movq	136(%rcx), %rax
	movq	%rax, 136(%rbx)  #rflags
	
	movq	144(%rcx), %rax
	movq	%rax, 48(%rbx)  #rsp
	
	movq	152(%rcx), %rax
	movq	%rax, 192(%rbx)  #ss
	
	retq*/
