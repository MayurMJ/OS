.section .text
.global switchTask
switchTask:
	pushq	%rax
	pushq   %rbx
	pushq   %rcx
	pushq   %rdx
	pushq   %rsi
	pushq   %rdi
	movq   %rsp, %rax
	pushq	%rax
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
	movq	%cr3, %rax
	pushq	%rax # pushing stuff is over
	movq	%rdi, %rax # from is copied to rax
	movq	%rbx, 8(%rax)
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
	movq	144(%rsp), %rbx # rax
	movq	152(%rsp), %rcx # ip , changed it to access the ret addr pushed into stack bfr switch call
	movq	96(%rsp), %rdx # rsp
	addq	$56, %rdx # might need to pop beyond the return addr, not sure
	movq	88(%rsp), %r8 # rbp
	movq	8(%rsp), %rdi # eflags
	movq	%rbx, (%rax)  # rax
	movq	%rdx, 48(%rax) # rsp
	movq	%r8, 56(%rax) # rbp
	movq	%rcx, 64(%rax) # ip
	movq	%rdi, 136(%rax) # eflags
	popq	%rbx #cr3
	movq	%rbx, 144(%rax)
	pushq	%rbx
	movq	%rsi, %rax # copying to ptr to rax
	movq	8(%rax), %rbx
	movq	16(%rax), %rcx
	movq	24(%rax), %rdx
	movq	32(%rax), %rsi
	movq	40(%rax), %rdi
	movq	56(%rax), %rbp
	movq    72(%rax), %r8
	movq    80(%rax), %r9
	movq    88(%rax), %r10
	movq    96(%rax), %r11
	movq    104(%rax), %r12
	movq    112(%rax), %r13
	movq    120(%rax), %r14
	movq    128(%rax), %r15
	pushq	%rax
	movq	136(%rax), %rax
	pushq	%rax
	popfq
	popq	%rax
	movq	48(%rax), %rsp # is it an issue that rsp is overwritten? how is the top of stack tracked?
	pushq	%rax # or have we switched to the new task's stack?  :O in that case rbp cannot be 0 in createTask
	movq	144(%rax), %rax
	movq	%rax, %cr3
	popq	%rax # rax contains "to" the ptr now
	pushq	%rax
	movq	64(%rax), %rax
	xchgq	(%rsp), %rax # rax is to's RAX and TOS has to's RIP at this point
	movq	(%rax), %rax
	retq
