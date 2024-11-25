	.file	"fun_mod_mersenne.cc"
	.text
	.p2align 4
	.globl	_Z18loop_regular_mod32jPjj
	.type	_Z18loop_regular_mod32jPjj, @function
_Z18loop_regular_mod32jPjj:
.LFB3393:
	.cfi_startproc
	testl	%edx, %edx
	je	.L4
	leal	-1(%rdx), %eax
	xorl	%r8d, %r8d
	leaq	4(%rsi,%rax,4), %rcx
	.p2align 4,,10
	.p2align 3
.L3:
	movl	(%rsi), %eax
	xorl	%edx, %edx
	addq	$4, %rsi
	divl	%edi
	addl	%edx, %r8d
	cmpq	%rsi, %rcx
	jne	.L3
	movl	%r8d, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	xorl	%r8d, %r8d
	movl	%r8d, %eax
	ret
	.cfi_endproc
.LFE3393:
	.size	_Z18loop_regular_mod32jPjj, .-_Z18loop_regular_mod32jPjj
	.p2align 4
	.globl	_Z19loop_mersenne_mod32jjPjj
	.type	_Z19loop_mersenne_mod32jjPjj, @function
_Z19loop_mersenne_mod32jjPjj:
.LFB3394:
	.cfi_startproc
	movl	%esi, %r9d
	movq	%rdx, %rsi
	movl	%ecx, %edx
	testl	%ecx, %ecx
	je	.L17
	leal	-1(%rcx), %eax
	cmpl	$2, %eax
	jbe	.L18
	movd	%edi, %xmm6
	shrl	$2, %ecx
	movq	%rsi, %rax
	pxor	%xmm2, %xmm2
	pshufd	$0, %xmm6, %xmm3
	subl	$1, %ecx
	movd	%r9d, %xmm8
	movdqa	.LC0(%rip), %xmm6
	salq	$4, %rcx
	movdqa	%xmm3, %xmm7
	leaq	16(%rsi,%rcx), %rcx
	psubd	%xmm6, %xmm7
	.p2align 4,,10
	.p2align 3
.L10:
	movdqu	(%rax), %xmm1
	movdqu	(%rax), %xmm0
	addq	$16, %rax
	psrld	%xmm8, %xmm1
	pand	%xmm3, %xmm0
	paddd	%xmm1, %xmm0
	movdqa	%xmm7, %xmm1
	movdqa	%xmm0, %xmm5
	movdqa	%xmm0, %xmm4
	psubd	%xmm6, %xmm5
	psubd	%xmm3, %xmm4
	pcmpgtd	%xmm5, %xmm1
	pand	%xmm1, %xmm0
	pandn	%xmm4, %xmm1
	por	%xmm0, %xmm1
	paddd	%xmm1, %xmm2
	cmpq	%rax, %rcx
	jne	.L10
	movdqa	%xmm2, %xmm0
	movl	%edx, %r8d
	psrldq	$8, %xmm0
	andl	$-4, %r8d
	paddd	%xmm0, %xmm2
	movdqa	%xmm2, %xmm0
	psrldq	$4, %xmm0
	paddd	%xmm0, %xmm2
	movd	%xmm2, %eax
	testb	$3, %dl
	je	.L20
.L9:
	movl	%r8d, %ecx
	movl	(%rsi,%rcx,4), %r11d
	movl	%r9d, %ecx
	movl	%r11d, %r10d
	shrl	%cl, %r11d
	andl	%edi, %r10d
	leal	(%r10,%r11), %ecx
	movl	%ecx, %r10d
	subl	%edi, %r10d
	cmpl	%ecx, %edi
	cmovbe	%r10d, %ecx
	addl	%ecx, %eax
	leal	1(%r8), %ecx
	cmpl	%ecx, %edx
	jbe	.L7
	movl	(%rsi,%rcx,4), %r11d
	movl	%edi, %r10d
	movl	%r9d, %ecx
	andl	%r11d, %r10d
	shrl	%cl, %r11d
	leal	(%r10,%r11), %ecx
	movl	%ecx, %r10d
	subl	%edi, %r10d
	cmpl	%ecx, %edi
	cmovbe	%r10d, %ecx
	addl	$2, %r8d
	addl	%ecx, %eax
	cmpl	%r8d, %edx
	jbe	.L7
	movl	(%rsi,%r8,4), %esi
	movl	%r9d, %ecx
	movl	%edi, %edx
	andl	%esi, %edx
	shrl	%cl, %esi
	addl	%esi, %edx
	movl	%edx, %ecx
	subl	%edi, %ecx
	cmpl	%edx, %edi
	cmovbe	%ecx, %edx
	addl	%edx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L17:
	xorl	%eax, %eax
.L7:
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	ret
.L18:
	xorl	%r8d, %r8d
	xorl	%eax, %eax
	jmp	.L9
	.cfi_endproc
.LFE3394:
	.size	_Z19loop_mersenne_mod32jjPjj, .-_Z19loop_mersenne_mod32jjPjj
	.p2align 4
	.globl	_Z18loop_regular_mod64mPmj
	.type	_Z18loop_regular_mod64mPmj, @function
_Z18loop_regular_mod64mPmj:
.LFB3395:
	.cfi_startproc
	testl	%edx, %edx
	je	.L24
	leal	-1(%rdx), %eax
	xorl	%r8d, %r8d
	leaq	8(%rsi,%rax,8), %rcx
	.p2align 4,,10
	.p2align 3
.L23:
	movq	(%rsi), %rax
	xorl	%edx, %edx
	addq	$8, %rsi
	divq	%rdi
	addq	%rdx, %r8
	cmpq	%rsi, %rcx
	jne	.L23
	movq	%r8, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L24:
	xorl	%r8d, %r8d
	movq	%r8, %rax
	ret
	.cfi_endproc
.LFE3395:
	.size	_Z18loop_regular_mod64mPmj, .-_Z18loop_regular_mod64mPmj
	.p2align 4
	.globl	_Z19loop_mersenne_mod64mmPmj
	.type	_Z19loop_mersenne_mod64mmPmj, @function
_Z19loop_mersenne_mod64mmPmj:
.LFB3396:
	.cfi_startproc
	movq	%rdi, %r8
	movl	%ecx, %eax
	testl	%ecx, %ecx
	je	.L30
	subl	$1, %eax
	movl	%esi, %ecx
	xorl	%r9d, %r9d
	leaq	8(%rdx,%rax,8), %rsi
	.p2align 4,,10
	.p2align 3
.L29:
	movq	(%rdx), %rdi
	movq	%rdi, %rax
	andq	%r8, %rdi
	shrq	%cl, %rax
	addq	%rdi, %rax
	movq	%rax, %rdi
	subq	%r8, %rdi
	cmpq	%r8, %rax
	cmovnb	%rdi, %rax
	addq	$8, %rdx
	addq	%rax, %r9
	cmpq	%rdx, %rsi
	jne	.L29
	movq	%r9, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L30:
	xorl	%r9d, %r9d
	movq	%r9, %rax
	ret
	.cfi_endproc
.LFE3396:
	.size	_Z19loop_mersenne_mod64mmPmj, .-_Z19loop_mersenne_mod64mmPmj
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.type	_GLOBAL__sub_I__Z18loop_regular_mod32jPjj, @function
_GLOBAL__sub_I__Z18loop_regular_mod32jPjj:
.LFB4042:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	_ZStL8__ioinit(%rip), %rbp
	movq	%rbp, %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	movq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE4042:
	.size	_GLOBAL__sub_I__Z18loop_regular_mod32jPjj, .-_GLOBAL__sub_I__Z18loop_regular_mod32jPjj
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z18loop_regular_mod32jPjj
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
