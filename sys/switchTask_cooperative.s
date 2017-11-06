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
	cli
	movq	48(%rsp), %rax
	movq	%rax, 144(%rdi)
	#-----------------------
	movq    56(%rsp), %rax
        movq    %rax, 136(%rdi)
	#-----------------------
	movq    64(%rsp), %rax
        movq    %rax, 128(%rdi)
        #-----------------------
	movq    72(%rsp), %rax
        movq    %rax, 120(%rdi)
        #-----------------------
	movq    80(%rsp), %rax
        movq    %rax, 112(%rdi)
        #-----------------------
	movq    88(%rsp), %rax
        movq    %rax, 104(%rdi)
        #-----------------------
	movq    96(%rsp), %rax
        movq    %rax, 96(%rdi)
        #-----------------------
	movq    104(%rsp), %rax
        movq    %rax, 88(%rdi)
        #-----------------------
	movq    112(%rsp), %rax
        movq    %rax, 56(%rdi)
        #-----------------------
	movq    120(%rsp), %rax
        movq    %rax, 32(%rdi)
        #-----------------------
	movq    128(%rsp), %rax
        movq    %rax, 40(%rdi)
        #-----------------------
	movq    136(%rsp), %rax
        movq    %rax, 24(%rdi)
        #-----------------------
	movq    144(%rsp), %rax
        movq    %rax, 16(%rdi)
        #-----------------------
	movq    152(%rsp), %rax
        movq    %rax, 8(%rdi)
        #-----------------------
	movq    160(%rsp), %rax
        movq    %rax, (%rdi)
        #-----------------------
	#-----------------------	
	# reset stack here? not sure
	#-----------------------
        #-----------------------
	movq	144(%rsi), %rax
	movq	%rax, (%r15)
	#-----------------------
	movq    136(%rsi), %rax
        movq    %rax, (%r14)
        #-----------------------
	movq    128(%rsi), %rax
        movq    %rax, (%r13)
        #-----------------------
	movq    120(%rsi), %rax
        movq    %rax, (%r12)
        #-----------------------
	movq    112(%rsi), %rax
        movq    %rax, (%r11)
        #-----------------------
	movq    104(%rsi), %rax
        movq    %rax, (%r10)
        #-----------------------
	movq    96(%rsi), %rax
        movq    %rax, (%r9)
        #-----------------------
	movq    88(%rsi), %rax
        movq    %rax, (%r8)
        #-----------------------
	movq    8(%rsi), %rax
        movq    %rax, (%rbx)
        #-----------------------
	movq    16(%rsi), %rax
        movq    %rax, (%rcx)
        #-----------------------
	movq    24(%rsi), %rax
        movq    %rax, (%rdx)
        #-----------------------
	#rsi,rdi,rsp,rbp,rax,rflags,cr3,rip
