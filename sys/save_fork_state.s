.section .text
.global saveState
saveState:
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
	
	retq
