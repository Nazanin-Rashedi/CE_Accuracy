	.file	"main_vp.cc"
	.section	.text.unlikely,"ax",@progbits
	.type	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc.part.3, @function
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc.part.3:
.LFB1990:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	(%rdi), %rax
	addq	-24(%rax), %rdi
	movl	32(%rdi), %esi
	orl	$1, %esi
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate
	popq	%rdx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1990:
	.size	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc.part.3, .-_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc.part.3
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"main_vp.cc"
.LC2:
	.string	"x.size() == y.size()"
	.text
	.p2align 4,,15
	.globl	_Z7dotprodRSt6vectorIdSaIdEES2_
	.type	_Z7dotprodRSt6vectorIdSaIdEES2_, @function
_Z7dotprodRSt6vectorIdSaIdEES2_:
.LFB1880:
	.cfi_startproc
	movq	(%rdi), %rcx
	movq	8(%rdi), %rdx
	movq	8(%rsi), %rax
	movq	(%rsi), %rdi
	subq	%rcx, %rdx
	sarq	$3, %rdx
	subq	%rdi, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	jne	.L4
	xorl	%eax, %eax
	testq	%rdx, %rdx
	vxorpd	%xmm0, %xmm0, %xmm0
	je	.L12
	.p2align 4,,10
	.p2align 3
.L8:
	vmovsd	(%rcx,%rax,8), %xmm1
	vmulsd	(%rdi,%rax,8), %xmm1, %xmm1
	addq	$1, %rax
	cmpq	%rdx, %rax
	vaddsd	%xmm1, %xmm0, %xmm0
	jne	.L8
	rep ret
.L12:
	rep ret
.L4:
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZZ7dotprodRSt6vectorIdSaIdEES2_E19__PRETTY_FUNCTION__, %ecx
	movl	$19, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
	.cfi_endproc
.LFE1880:
	.size	_Z7dotprodRSt6vectorIdSaIdEES2_, .-_Z7dotprodRSt6vectorIdSaIdEES2_
	.section	.rodata.str1.1
.LC3:
	.string	"vector::_M_fill_insert"
	.section	.text._ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd,"axG",@progbits,_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd
	.type	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd, @function
_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd:
.LFB1928:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	movq	%rdx, %r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$32, %rsp
	.cfi_offset 3, -56
	testq	%rdx, %rdx
	je	.L125
	movq	8(%rdi), %r14
	movq	%rdi, %r13
	movq	%rsi, %rbx
	movq	16(%rdi), %rax
	subq	%r14, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	ja	.L16
	movq	%r14, %rax
	vmovsd	(%rcx), %xmm0
	subq	%rsi, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	jb	.L127
	subq	%rax, %r12
	je	.L43
	movq	%r14, %rdx
	movq	%r12, %rcx
	andl	$31, %edx
	shrq	$3, %rdx
	negq	%rdx
	andl	$3, %edx
	cmpq	%r12, %rdx
	cmova	%r12, %rdx
	cmpq	$6, %r12
	ja	.L128
.L34:
	vmovsd	%xmm0, (%r14)
	leaq	-1(%r12), %rsi
	cmpq	$1, %rcx
	leaq	8(%r14), %rdx
	jbe	.L36
	vmovsd	%xmm0, 8(%r14)
	leaq	-2(%r12), %rsi
	cmpq	$2, %rcx
	leaq	16(%r14), %rdx
	jbe	.L36
	vmovsd	%xmm0, 16(%r14)
	leaq	-3(%r12), %rsi
	cmpq	$3, %rcx
	leaq	24(%r14), %rdx
	jbe	.L36
	vmovsd	%xmm0, 24(%r14)
	leaq	-4(%r12), %rsi
	cmpq	$4, %rcx
	leaq	32(%r14), %rdx
	jbe	.L36
	vmovsd	%xmm0, 32(%r14)
	leaq	-5(%r12), %rsi
	cmpq	$5, %rcx
	leaq	40(%r14), %rdx
	jbe	.L36
	vmovsd	%xmm0, 40(%r14)
	leaq	-6(%r12), %rsi
	leaq	48(%r14), %rdx
.L36:
	cmpq	%rcx, %r12
	je	.L43
.L35:
	movq	%r12, %r10
	subq	%rcx, %r10
	movq	%r10, %r9
	shrq	$2, %r9
	leaq	0(,%r9,4), %r8
	testq	%r8, %r8
	je	.L38
	leaq	(%r14,%rcx,8), %rdi
	xorl	%ecx, %ecx
	vmovddup	%xmm0, %xmm1
	vinsertf128	$1, %xmm1, %ymm1, %ymm1
.L44:
	addq	$1, %rcx
	vmovapd	%ymm1, (%rdi)
	addq	$32, %rdi
	cmpq	%r9, %rcx
	jb	.L44
	leaq	(%rdx,%r8,8), %rdx
	subq	%r8, %rsi
	cmpq	%r8, %r10
	je	.L43
.L38:
	cmpq	$1, %rsi
	vmovsd	%xmm0, (%rdx)
	je	.L43
	cmpq	$2, %rsi
	vmovsd	%xmm0, 8(%rdx)
	je	.L43
	vmovsd	%xmm0, 16(%rdx)
.L43:
	leaq	(%r14,%r12,8), %rdi
	xorl	%r12d, %r12d
	testq	%rax, %rax
	movq	%rdi, 8(%r13)
	jne	.L129
.L45:
	addq	%rdi, %r12
	cmpq	%rbx, %r14
	movq	%r12, 8(%r13)
	je	.L123
	leaq	8(%rbx), %rax
	movq	%rbx, %rdx
	movq	%r14, %rsi
	andl	$31, %edx
	subq	%rax, %rsi
	shrq	$3, %rdx
	shrq	$3, %rsi
	negq	%rdx
	addq	$1, %rsi
	andl	$3, %edx
	movq	%rsi, %rcx
	cmpq	%rsi, %rdx
	cmova	%rsi, %rdx
	cmpq	$6, %rsi
	ja	.L130
.L46:
	cmpq	$1, %rcx
	vmovsd	%xmm0, (%rbx)
	jbe	.L48
	vmovsd	%xmm0, 8(%rbx)
	leaq	16(%rbx), %rax
	cmpq	$2, %rcx
	jbe	.L48
	vmovsd	%xmm0, 16(%rbx)
	leaq	24(%rbx), %rax
	cmpq	$3, %rcx
	jbe	.L48
	vmovsd	%xmm0, 24(%rbx)
	leaq	32(%rbx), %rax
	cmpq	$4, %rcx
	jbe	.L48
	vmovsd	%xmm0, 32(%rbx)
	leaq	40(%rbx), %rax
	cmpq	$5, %rcx
	jbe	.L48
	vmovsd	%xmm0, 40(%rbx)
	leaq	48(%rbx), %rax
.L48:
	cmpq	%rcx, %rsi
	je	.L123
.L47:
	subq	%rcx, %rsi
	movq	%rsi, %rdi
	shrq	$2, %rdi
	leaq	0(,%rdi,4), %r8
	testq	%r8, %r8
	je	.L50
	leaq	(%rbx,%rcx,8), %rcx
	xorl	%edx, %edx
	vmovddup	%xmm0, %xmm1
	vinsertf128	$1, %xmm1, %ymm1, %ymm1
.L55:
	addq	$1, %rdx
	vmovapd	%ymm1, (%rcx)
	addq	$32, %rcx
	cmpq	%rdi, %rdx
	jb	.L55
	leaq	(%rax,%r8,8), %rax
	cmpq	%r8, %rsi
	je	.L123
.L50:
	leaq	8(%rax), %rdx
	vmovsd	%xmm0, (%rax)
	cmpq	%rdx, %r14
	je	.L123
	leaq	16(%rax), %rdx
	vmovsd	%xmm0, 8(%rax)
	cmpq	%rdx, %r14
	je	.L123
	vmovsd	%xmm0, 16(%rax)
	vzeroupper
.L125:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L127:
	.cfi_restore_state
	salq	$3, %r12
	movq	%r14, %r15
	movq	%r14, %rax
	movq	%r12, %rdx
	subq	%r12, %r15
	sarq	$3, %rdx
	testq	%rdx, %rdx
	je	.L18
	salq	$3, %rdx
	movq	%r15, %rsi
	movq	%r14, %rdi
	vmovsd	%xmm0, 24(%rsp)
	call	memmove
	movq	8(%r13), %rax
	vmovsd	24(%rsp), %xmm0
.L18:
	movq	%r15, %rsi
	addq	%r12, %rax
	subq	%rbx, %rsi
	movq	%rax, 8(%r13)
	sarq	$3, %rsi
	testq	%rsi, %rsi
	je	.L19
	leaq	0(,%rsi,8), %rdx
	movq	%r14, %rdi
	movq	%rbx, %rsi
	subq	%rdx, %rdi
	vmovsd	%xmm0, 24(%rsp)
	call	memmove
	vmovsd	24(%rsp), %xmm0
.L19:
	addq	%rbx, %r12
	cmpq	%rbx, %r12
	je	.L125
	leaq	8(%rbx), %rax
	movq	%rbx, %rdx
	movq	%r12, %rsi
	andl	$31, %edx
	subq	%rax, %rsi
	shrq	$3, %rdx
	shrq	$3, %rsi
	negq	%rdx
	addq	$1, %rsi
	andl	$3, %edx
	movq	%rsi, %rcx
	cmpq	%rdx, %rsi
	cmovbe	%rsi, %rdx
	cmpq	$6, %rsi
	ja	.L131
.L21:
	cmpq	$1, %rcx
	vmovsd	%xmm0, (%rbx)
	jbe	.L23
	vmovsd	%xmm0, 8(%rbx)
	leaq	16(%rbx), %rax
	cmpq	$2, %rcx
	jbe	.L23
	vmovsd	%xmm0, 16(%rbx)
	leaq	24(%rbx), %rax
	cmpq	$3, %rcx
	jbe	.L23
	vmovsd	%xmm0, 24(%rbx)
	leaq	32(%rbx), %rax
	cmpq	$4, %rcx
	jbe	.L23
	vmovsd	%xmm0, 32(%rbx)
	leaq	40(%rbx), %rax
	cmpq	$5, %rcx
	jbe	.L23
	vmovsd	%xmm0, 40(%rbx)
	leaq	48(%rbx), %rax
.L23:
	cmpq	%rcx, %rsi
	je	.L125
.L22:
	subq	%rcx, %rsi
	movq	%rsi, %r8
	shrq	$2, %r8
	leaq	0(,%r8,4), %rdi
	testq	%rdi, %rdi
	je	.L25
	leaq	(%rbx,%rcx,8), %rcx
	xorl	%edx, %edx
	vmovddup	%xmm0, %xmm1
	vinsertf128	$1, %xmm1, %ymm1, %ymm1
.L30:
	addq	$1, %rdx
	vmovapd	%ymm1, (%rcx)
	addq	$32, %rcx
	cmpq	%rdx, %r8
	ja	.L30
	leaq	(%rax,%rdi,8), %rax
	cmpq	%rsi, %rdi
	jne	.L132
	.p2align 4,,10
	.p2align 3
.L123:
	vzeroupper
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	.cfi_restore_state
	movabsq	$2305843009213693951, %rdx
	movq	(%rdi), %rsi
	movq	%rdx, %rax
	subq	%rsi, %r14
	sarq	$3, %r14
	subq	%r14, %rax
	cmpq	%rax, %r12
	ja	.L133
	cmpq	%r12, %r14
	movq	%r12, %rax
	cmovnb	%r14, %rax
	addq	%rax, %r14
	jnc	.L134
.L57:
	movq	%rbx, %rdx
	movq	$-8, %r14
	subq	%rsi, %rdx
	sarq	$3, %rdx
.L73:
	movq	%r14, %rdi
	movq	%rcx, 8(%rsp)
	movq	%rdx, 16(%rsp)
	call	_Znwm
	movq	0(%r13), %rsi
	movq	%rbx, %r10
	movq	16(%rsp), %rdx
	movq	%rax, %r15
	movq	8(%rsp), %rcx
	subq	%rsi, %r10
	sarq	$3, %r10
	movq	%r10, 24(%rsp)
.L58:
	leaq	(%r15,%rdx,8), %rdx
	vmovsd	(%rcx), %xmm0
	movq	%r12, %r8
	movq	%rdx, %rax
	andl	$31, %eax
	shrq	$3, %rax
	negq	%rax
	andl	$3, %eax
	cmpq	%r12, %rax
	cmova	%r12, %rax
	cmpq	$6, %r12
	ja	.L135
.L59:
	vmovsd	%xmm0, (%rdx)
	leaq	-1(%r12), %r9
	cmpq	$1, %r8
	leaq	8(%rdx), %rcx
	jbe	.L61
	vmovsd	%xmm0, 8(%rdx)
	leaq	-2(%r12), %r9
	cmpq	$2, %r8
	leaq	16(%rdx), %rcx
	jbe	.L61
	vmovsd	%xmm0, 16(%rdx)
	leaq	-3(%r12), %r9
	cmpq	$3, %r8
	leaq	24(%rdx), %rcx
	jbe	.L61
	vmovsd	%xmm0, 24(%rdx)
	leaq	-4(%r12), %r9
	cmpq	$4, %r8
	leaq	32(%rdx), %rcx
	jbe	.L61
	vmovsd	%xmm0, 32(%rdx)
	leaq	-5(%r12), %r9
	cmpq	$5, %r8
	leaq	40(%rdx), %rcx
	jbe	.L61
	vmovsd	%xmm0, 40(%rdx)
	leaq	-6(%r12), %r9
	leaq	48(%rdx), %rcx
.L61:
	cmpq	%r8, %r12
	je	.L62
.L60:
	movq	%r12, %r11
	subq	%r8, %r11
	movq	%r11, %rdi
	shrq	$2, %rdi
	leaq	0(,%rdi,4), %rax
	testq	%rax, %rax
	je	.L63
	leaq	(%rdx,%r8,8), %r8
	xorl	%edx, %edx
	vmovddup	%xmm0, %xmm1
	vinsertf128	$1, %xmm1, %ymm1, %ymm1
.L68:
	addq	$1, %rdx
	vmovapd	%ymm1, (%r8)
	addq	$32, %r8
	cmpq	%rdi, %rdx
	jb	.L68
	leaq	(%rcx,%rax,8), %rcx
	subq	%rax, %r9
	cmpq	%rax, %r11
	je	.L124
	vzeroupper
.L63:
	cmpq	$1, %r9
	vmovsd	%xmm0, (%rcx)
	je	.L62
	cmpq	$2, %r9
	vmovsd	%xmm0, 8(%rcx)
	je	.L62
	vmovsd	%xmm0, 16(%rcx)
.L62:
	movq	24(%rsp), %rcx
	salq	$3, %rcx
	testq	%r10, %r10
	je	.L70
	movq	%rcx, %rdx
	movq	%r15, %rdi
	movq	%rcx, 24(%rsp)
	call	memmove
	movq	24(%rsp), %rcx
.L70:
	movq	8(%r13), %rax
	addq	%r15, %rcx
	leaq	(%rcx,%r12,8), %rcx
	xorl	%r12d, %r12d
	subq	%rbx, %rax
	sarq	$3, %rax
	testq	%rax, %rax
	je	.L71
	leaq	0(,%rax,8), %r12
	movq	%rcx, %rdi
	movq	%rbx, %rsi
	movq	%r12, %rdx
	call	memmove
	movq	%rax, %rcx
.L71:
	movq	0(%r13), %rdi
	addq	%rcx, %r12
	testq	%rdi, %rdi
	je	.L72
	call	_ZdlPv
.L72:
	addq	%r15, %r14
	movq	%r15, 0(%r13)
	movq	%r12, 8(%r13)
	movq	%r14, 16(%r13)
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L129:
	.cfi_restore_state
	leaq	0(,%rax,8), %r12
	movq	%rbx, %rsi
	vmovsd	%xmm0, 24(%rsp)
	movq	%r12, %rdx
	vzeroupper
	call	memmove
	movq	8(%r13), %rdi
	vmovsd	24(%rsp), %xmm0
	jmp	.L45
	.p2align 4,,10
	.p2align 3
.L128:
	testq	%rdx, %rdx
	jne	.L136
	xorl	%ecx, %ecx
	movq	%r12, %rsi
	movq	%r14, %rdx
	jmp	.L35
	.p2align 4,,10
	.p2align 3
.L130:
	testq	%rdx, %rdx
	jne	.L137
	xorl	%ecx, %ecx
	movq	%rbx, %rax
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L132:
	vzeroupper
.L25:
	leaq	8(%rax), %rdx
	vmovsd	%xmm0, (%rax)
	cmpq	%rdx, %r12
	je	.L125
	leaq	16(%rax), %rdx
	vmovsd	%xmm0, 8(%rax)
	cmpq	%rdx, %r12
	je	.L125
	vmovsd	%xmm0, 16(%rax)
	jmp	.L125
	.p2align 4,,10
	.p2align 3
.L135:
	testq	%rax, %rax
	jne	.L138
	xorl	%r8d, %r8d
	movq	%r12, %r9
	movq	%rdx, %rcx
	jmp	.L60
	.p2align 4,,10
	.p2align 3
.L134:
	cmpq	%rdx, %r14
	ja	.L57
	movq	%rbx, %rdx
	subq	%rsi, %rdx
	sarq	$3, %rdx
	testq	%r14, %r14
	jne	.L139
	movq	%rdx, 24(%rsp)
	movq	%rdx, %r10
	xorl	%r14d, %r14d
	xorl	%r15d, %r15d
	jmp	.L58
	.p2align 4,,10
	.p2align 3
.L131:
	testq	%rdx, %rdx
	jne	.L140
	xorl	%ecx, %ecx
	movq	%rbx, %rax
	jmp	.L22
	.p2align 4,,10
	.p2align 3
.L124:
	vzeroupper
	jmp	.L62
.L138:
	movq	%rax, %r8
	jmp	.L59
.L139:
	salq	$3, %r14
	jmp	.L73
.L137:
	movq	%rdx, %rcx
	jmp	.L46
.L136:
	movq	%rdx, %rcx
	jmp	.L34
.L133:
	movl	$.LC3, %edi
	call	_ZSt20__throw_length_errorPKc
.L140:
	movq	%rdx, %rcx
	jmp	.L21
	.cfi_endproc
.LFE1928:
	.size	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd, .-_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd
	.text
	.p2align 4,,15
	.globl	_Z11fill_vectorRSt6vectorIdSaIdEEj
	.type	_Z11fill_vectorRSt6vectorIdSaIdEEj, @function
_Z11fill_vectorRSt6vectorIdSaIdEEj:
.LFB1879:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%esi, %edx
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	.cfi_offset 12, -24
	movq	%rdi, %r12
	pushq	%rbx
	.cfi_offset 3, -32
	movl	%esi, %ebx
	andq	$-32, %rsp
	subq	$32, %rsp
	movq	8(%rdi), %rsi
	movq	$0, (%rsp)
	movq	(%rdi), %rcx
	movq	%rsi, %rax
	subq	%rcx, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	ja	.L174
	jnb	.L144
	leaq	(%rcx,%rdx,8), %rax
	movq	%rax, 8(%rdi)
.L144:
	testl	%ebx, %ebx
	je	.L172
	movq	(%r12), %rax
	movl	%ebx, %ecx
	movq	%rax, %rdx
	salq	$60, %rdx
	shrq	$63, %rdx
	cmpl	%ebx, %edx
	cmova	%ebx, %edx
	cmpl	$5, %ebx
	ja	.L175
.L145:
	vmovsd	.LC4(%rip), %xmm5
	cmpl	$1, %ecx
	vmovsd	%xmm5, (%rax)
	jbe	.L164
	vmovsd	.LC5(%rip), %xmm6
	cmpl	$2, %ecx
	vmovsd	%xmm6, 8(%rax)
	jbe	.L165
	vmovsd	.LC6(%rip), %xmm7
	cmpl	$3, %ecx
	vmovsd	%xmm7, 16(%rax)
	jbe	.L166
	vmovsd	.LC7(%rip), %xmm2
	cmpl	$4, %ecx
	vmovsd	%xmm2, 24(%rax)
	jbe	.L167
	vmovsd	.LC8(%rip), %xmm3
	movl	$5, %edx
	vmovsd	%xmm3, 32(%rax)
.L147:
	cmpl	%ecx, %ebx
	je	.L172
.L146:
	movl	%ebx, %r9d
	movl	%ecx, %esi
	subl	%ecx, %r9d
	movl	%r9d, %edi
	shrl	$2, %edi
	leal	0(,%rdi,4), %r8d
	testl	%r8d, %r8d
	je	.L149
	leal	1(%rdx), %ecx
	vmovd	%edx, %xmm5
	vmovdqa	.LC9(%rip), %xmm6
	vpinsrd	$1, %ecx, %xmm5, %xmm1
	leal	2(%rdx), %r11d
	vmovdqa	.LC10(%rip), %xmm5
	vmovapd	.LC11(%rip), %xmm3
	leal	3(%rdx), %r10d
	vmovd	%r11d, %xmm0
	leaq	(%rax,%rsi,8), %rcx
	xorl	%esi, %esi
	vpinsrd	$1, %r10d, %xmm0, %xmm0
	vxorpd	%xmm2, %xmm2, %xmm2
	vpunpcklqdq	%xmm0, %xmm1, %xmm0
.L161:
	vpaddd	%xmm6, %xmm0, %xmm7
	vpaddd	%xmm5, %xmm0, %xmm0
	addl	$1, %esi
	vcvtdq2pd	%xmm0, %xmm1
	vcmpltpd	%xmm2, %xmm1, %xmm4
	vpshufd	$238, %xmm0, %xmm0
	vcvtdq2pd	%xmm0, %xmm0
	addq	$32, %rcx
	vandpd	%xmm3, %xmm4, %xmm4
	vaddpd	%xmm4, %xmm1, %xmm1
	vmovapd	%xmm1, -32(%rcx)
	vcmpltpd	%xmm2, %xmm0, %xmm1
	vandpd	%xmm3, %xmm1, %xmm1
	vaddpd	%xmm1, %xmm0, %xmm0
	vmovapd	%xmm0, -16(%rcx)
	cmpl	%esi, %edi
	jbe	.L176
	vmovdqa	%xmm7, %xmm0
	jmp	.L161
	.p2align 4,,10
	.p2align 3
.L176:
	addl	%r8d, %edx
	cmpl	%r8d, %r9d
	je	.L172
.L149:
	leal	1(%rdx), %ecx
	movl	%edx, %edi
	movl	%ecx, %esi
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%rsi, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rdi,8)
	jbe	.L172
	leal	2(%rdx), %ecx
	movl	%ecx, %edi
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%rdi, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rsi,8)
	jbe	.L172
	addl	$3, %edx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rdi,8)
.L172:
	leaq	-16(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L175:
	.cfi_restore_state
	testl	%edx, %edx
	jne	.L177
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	jmp	.L146
	.p2align 4,,10
	.p2align 3
.L174:
	subq	%rax, %rdx
	movq	%rsp, %rcx
	call	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd
	jmp	.L144
	.p2align 4,,10
	.p2align 3
.L167:
	movl	$4, %edx
	jmp	.L147
	.p2align 4,,10
	.p2align 3
.L164:
	movl	$1, %edx
	jmp	.L147
	.p2align 4,,10
	.p2align 3
.L165:
	movl	$2, %edx
	jmp	.L147
	.p2align 4,,10
	.p2align 3
.L166:
	movl	$3, %edx
	jmp	.L147
.L177:
	movl	%edx, %ecx
	jmp	.L145
	.cfi_endproc
.LFE1879:
	.size	_Z11fill_vectorRSt6vectorIdSaIdEEj, .-_Z11fill_vectorRSt6vectorIdSaIdEEj
	.section	.rodata.str1.1
.LC12:
	.string	"# "
.LC13:
	.string	" =?= "
.LC16:
	.string	"std::fabs(r1 - r2) < 1E-17"
	.text
	.p2align 4,,15
	.globl	_Z5checkj
	.type	_Z5checkj, @function
_Z5checkj:
.LFB1881:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1881
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r14
	pushq	%r13
	pushq	%r12
	.cfi_offset 14, -24
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	movl	%edi, %r12d
	pushq	%rbx
	.cfi_offset 3, -48
	movl	%edi, %ebx
	andq	$-32, %rsp
	subq	$272, %rsp
	testq	%r12, %r12
	movq	$0, 144(%rsp)
	movq	$0, 152(%rsp)
	movq	$0, 160(%rsp)
	movq	$0, 176(%rsp)
	movq	$0, 184(%rsp)
	movq	$0, 192(%rsp)
	movq	$0, 80(%rsp)
	jne	.L389
	xorl	%r8d, %r8d
	xorl	%esi, %esi
	xorl	%r13d, %r13d
.L179:
	testl	%ebx, %ebx
	je	.L198
	movq	144(%rsp), %rax
	movl	%ebx, %ecx
	movq	%rax, %rdx
	salq	$60, %rdx
	shrq	$63, %rdx
	cmpl	%ebx, %edx
	cmova	%ebx, %edx
	cmpl	$5, %ebx
	ja	.L390
.L183:
	vmovsd	.LC4(%rip), %xmm4
	cmpl	$1, %ecx
	vmovsd	%xmm4, (%rax)
	jbe	.L302
	vmovsd	.LC5(%rip), %xmm5
	cmpl	$2, %ecx
	vmovsd	%xmm5, 8(%rax)
	jbe	.L303
	vmovsd	.LC6(%rip), %xmm4
	cmpl	$3, %ecx
	vmovsd	%xmm4, 16(%rax)
	jbe	.L304
	vmovsd	.LC7(%rip), %xmm5
	cmpl	$4, %ecx
	vmovsd	%xmm5, 24(%rax)
	jbe	.L305
	vmovsd	.LC8(%rip), %xmm4
	movl	$5, %edx
	vmovsd	%xmm4, 32(%rax)
.L185:
	cmpl	%ecx, %ebx
	je	.L198
.L184:
	movl	%ebx, %r11d
	movl	%ecx, %edi
	subl	%ecx, %r11d
	movl	%r11d, %r9d
	shrl	$2, %r9d
	leal	0(,%r9,4), %r10d
	testl	%r10d, %r10d
	je	.L187
	leal	2(%rdx), %r14d
	vmovd	%edx, %xmm5
	vmovdqa	.LC9(%rip), %xmm6
	leal	1(%rdx), %ecx
	vmovd	%r14d, %xmm1
	vmovdqa	.LC10(%rip), %xmm3
	leal	3(%rdx), %r14d
	vpinsrd	$1, %ecx, %xmm5, %xmm0
	vxorpd	%xmm4, %xmm4, %xmm4
	vpinsrd	$1, %r14d, %xmm1, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm1
	leaq	(%rax,%rdi,8), %rcx
	vmovapd	.LC11(%rip), %xmm0
	xorl	%edi, %edi
.L199:
	vpaddd	%xmm6, %xmm1, %xmm7
	vpaddd	%xmm3, %xmm1, %xmm1
	addl	$1, %edi
	vcvtdq2pd	%xmm1, %xmm2
	vcmpltpd	%xmm4, %xmm2, %xmm5
	vpshufd	$238, %xmm1, %xmm1
	vcvtdq2pd	%xmm1, %xmm1
	addq	$32, %rcx
	vandpd	%xmm0, %xmm5, %xmm5
	vaddpd	%xmm5, %xmm2, %xmm2
	vmovapd	%xmm2, -32(%rcx)
	vcmpltpd	%xmm4, %xmm1, %xmm2
	vandpd	%xmm0, %xmm2, %xmm2
	vaddpd	%xmm2, %xmm1, %xmm1
	vmovapd	%xmm1, -16(%rcx)
	cmpl	%r9d, %edi
	jnb	.L391
	vmovdqa	%xmm7, %xmm1
	jmp	.L199
	.p2align 4,,10
	.p2align 3
.L391:
	addl	%r10d, %edx
	cmpl	%r10d, %r11d
	je	.L198
.L187:
	leal	1(%rdx), %ecx
	movl	%edx, %r9d
	movl	%ecx, %edi
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%rdi, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%r9,8)
	jbe	.L198
	leal	2(%rdx), %ecx
	movl	%ecx, %r9d
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%r9, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rdi,8)
	jbe	.L198
	addl	$3, %edx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%r9,8)
.L198:
	movq	$0, 112(%rsp)
	cmpq	%r8, %r12
	ja	.L392
	jnb	.L203
	leaq	0(%r13,%r12,8), %rax
	movq	%rax, 184(%rsp)
.L203:
	testl	%ebx, %ebx
	je	.L221
	movq	176(%rsp), %rax
	movl	%ebx, %ecx
	movq	%rax, %rdx
	salq	$60, %rdx
	shrq	$63, %rdx
	cmpl	%ebx, %edx
	cmova	%ebx, %edx
	cmpl	$5, %ebx
	ja	.L393
.L206:
	vmovsd	.LC4(%rip), %xmm5
	cmpl	$1, %ecx
	vmovsd	%xmm5, (%rax)
	jbe	.L308
	vmovsd	.LC5(%rip), %xmm4
	cmpl	$2, %ecx
	vmovsd	%xmm4, 8(%rax)
	jbe	.L309
	vmovsd	.LC6(%rip), %xmm5
	cmpl	$3, %ecx
	vmovsd	%xmm5, 16(%rax)
	jbe	.L310
	vmovsd	.LC7(%rip), %xmm4
	cmpl	$4, %ecx
	vmovsd	%xmm4, 24(%rax)
	jbe	.L311
	vmovsd	.LC8(%rip), %xmm5
	movl	$5, %edx
	vmovsd	%xmm5, 32(%rax)
.L208:
	cmpl	%ecx, %ebx
	je	.L221
.L207:
	movl	%ebx, %r9d
	movl	%ecx, %esi
	subl	%ecx, %r9d
	movl	%r9d, %edi
	shrl	$2, %edi
	leal	0(,%rdi,4), %r8d
	testl	%r8d, %r8d
	je	.L210
	leal	2(%rdx), %r14d
	vmovd	%edx, %xmm4
	vmovdqa	.LC9(%rip), %xmm6
	leal	1(%rdx), %ecx
	vmovd	%r14d, %xmm1
	vmovdqa	.LC10(%rip), %xmm3
	leal	3(%rdx), %r10d
	vpinsrd	$1, %ecx, %xmm4, %xmm0
	vxorpd	%xmm4, %xmm4, %xmm4
	vpinsrd	$1, %r10d, %xmm1, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm1
	leaq	(%rax,%rsi,8), %rcx
	vmovapd	.LC11(%rip), %xmm0
	xorl	%esi, %esi
.L222:
	vpaddd	%xmm6, %xmm1, %xmm7
	vpaddd	%xmm3, %xmm1, %xmm1
	addl	$1, %esi
	vcvtdq2pd	%xmm1, %xmm2
	vcmpltpd	%xmm4, %xmm2, %xmm5
	vpshufd	$238, %xmm1, %xmm1
	vcvtdq2pd	%xmm1, %xmm1
	addq	$32, %rcx
	vandpd	%xmm0, %xmm5, %xmm5
	vaddpd	%xmm5, %xmm2, %xmm2
	vmovapd	%xmm2, -32(%rcx)
	vcmpltpd	%xmm4, %xmm1, %xmm2
	vandpd	%xmm0, %xmm2, %xmm2
	vaddpd	%xmm2, %xmm1, %xmm1
	vmovapd	%xmm1, -16(%rcx)
	cmpl	%edi, %esi
	jnb	.L394
	vmovdqa	%xmm7, %xmm1
	jmp	.L222
	.p2align 4,,10
	.p2align 3
.L394:
	addl	%r8d, %edx
	cmpl	%r9d, %r8d
	je	.L221
.L210:
	leal	1(%rdx), %ecx
	movl	%edx, %edi
	movl	%ecx, %esi
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%rsi, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rdi,8)
	jbe	.L221
	leal	2(%rdx), %ecx
	movl	%ecx, %edi
	cmpl	%ecx, %ebx
	vcvtsi2sdq	%rdi, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rsi,8)
	jbe	.L221
	addl	$3, %edx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vmovsd	%xmm0, (%rax,%rdi,8)
.L221:
	leaq	208(%rsp), %rdi
.LEHB0:
	call	cmeasure_start
.LEHE0:
	movq	144(%rsp), %rcx
	movq	152(%rsp), %rdx
	movq	176(%rsp), %rsi
	movq	184(%rsp), %rax
	subq	%rcx, %rdx
	sarq	$3, %rdx
	subq	%rsi, %rax
	sarq	$3, %rax
	cmpq	%rax, %rdx
	jne	.L224
	xorl	%eax, %eax
	testq	%rdx, %rdx
	movq	$0, (%rsp)
	je	.L226
	.p2align 4,,10
	.p2align 3
.L357:
	vmovsd	(%rcx,%rax,8), %xmm0
	vmulsd	(%rsi,%rax,8), %xmm0, %xmm0
	addq	$1, %rax
	cmpq	%rax, %rdx
	vaddsd	(%rsp), %xmm0, %xmm6
	vmovsd	%xmm6, (%rsp)
	jne	.L357
.L226:
	leaq	208(%rsp), %rdi
.LEHB1:
	call	cmeasure_stop
.LEHE1:
	leaq	240(%rsp), %rdi
.LEHB2:
	call	cmeasure_start
.LEHE2:
	movl	%ebx, %esi
	movq	176(%rsp), %rdi
	andl	$7, %esi
	movq	144(%rsp), %rdx
	je	.L312
	movq	$0, 8(%rsp)
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L233:
	vmovsd	(%rdx,%rax,8), %xmm0
	vmulsd	(%rdi,%rax,8), %xmm0, %xmm0
	addq	$1, %rax
	cmpl	%eax, %esi
	vaddsd	8(%rsp), %xmm0, %xmm7
	vmovsd	%xmm7, 8(%rsp)
	jg	.L233
.L232:
	cmpl	%esi, %ebx
	jle	.L234
	movslq	%esi, %rcx
	subl	$1, %ebx
	vxorpd	%xmm0, %xmm0, %xmm0
	salq	$3, %rcx
	subl	%esi, %ebx
	vmovapd	%xmm0, %xmm1
	leaq	(%rdi,%rcx), %rax
	shrl	$2, %ebx
	addq	%rdx, %rcx
	leaq	32(%rcx), %rdx
	salq	$5, %rbx
	leaq	(%rdx,%rbx), %rsi
	jmp	.L244
	.p2align 4,,10
	.p2align 3
.L395:
	addq	$32, %rdx
.L244:
.L342:
.L343:
.L344:
.L319:
	vmovapd	(%rcx), %xmm3
	addq	$32, %rax
	vmovapd	16(%rcx), %xmm2
	movq	%rdx, %rcx
	vmulpd	-32(%rax), %xmm3, %xmm3
	vmulpd	-16(%rax), %xmm2, %xmm2
	cmpq	%rsi, %rdx
	vaddpd	%xmm3, %xmm1, %xmm1
	vaddpd	%xmm2, %xmm0, %xmm0
	jne	.L395
.L341:
	vaddpd	%xmm0, %xmm1, %xmm0
	vhaddpd	%xmm0, %xmm0, %xmm0
	vaddsd	8(%rsp), %xmm0, %xmm4
	vmovsd	%xmm4, 8(%rsp)
.L234:
	leaq	240(%rsp), %rdi
.LEHB3:
	call	cmeasure_stop
.LEHE3:
	movl	$2, %edx
	movl	$.LC12, %esi
	movl	$_ZSt4cout, %edi
.LEHB4:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
.LEHE4:
	vmovsd	(%rsp), %xmm0
	movl	$_ZSt4cout, %edi
.LEHB5:
	call	_ZNSo9_M_insertIdEERSoT_
.LEHE5:
	movl	$5, %edx
	movl	$.LC13, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
.LEHB6:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
.LEHE6:
	vmovsd	8(%rsp), %xmm0
	movq	%rbx, %rdi
.LEHB7:
	call	_ZNSo9_M_insertIdEERSoT_
.LEHE7:
	movq	%rax, %r13
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r13,%rax), %rbx
	testq	%rbx, %rbx
	je	.L396
	cmpb	$0, 56(%rbx)
	je	.L259
	movzbl	67(%rbx), %eax
.L260:
	movsbl	%al, %esi
	movq	%r13, %rdi
.LEHB8:
	call	_ZNSo3putEc
.LEHE8:
	movq	%rax, %rdi
.LEHB9:
	call	_ZNSo5flushEv
.LEHE9:
	vmovsd	(%rsp), %xmm4
	vmovsd	.LC14(%rip), %xmm1
	vsubsd	8(%rsp), %xmm4, %xmm0
	vandpd	%xmm1, %xmm0, %xmm0
	vmovsd	.LC15(%rip), %xmm1
	vucomisd	%xmm0, %xmm1
	jbe	.L397
	leaq	240(%rsp), %rdi
.LEHB10:
	call	cmeasure_total_ns
.LEHE10:
	leaq	208(%rsp), %rdi
	movq	%rax, %rbx
.LEHB11:
	call	cmeasure_total_ns
.LEHE11:
	movq	%rax, %r13
	movq	_ZSt4cout(%rip), %rax
	movq	%r12, %rsi
	movl	$_ZSt4cout, %edi
	movq	-24(%rax), %rax
	movq	$8, _ZSt4cout+16(%rax)
.LEHB12:
	call	_ZNSo9_M_insertImEERSoT_
.LEHE12:
	movl	$1, %edx
	movq	%rax, %rdi
	movb	$32, 16(%rsp)
	leaq	16(%rsp), %rsi
.LEHB13:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
.LEHE13:
	movq	%rax, %rdi
	movq	(%rax), %rax
	movq	%r13, %rsi
	movq	-24(%rax), %rax
	movq	$8, 16(%rdi,%rax)
.LEHB14:
	call	_ZNSo9_M_insertIlEERSoT_
.LEHE14:
	movl	$1, %edx
	movq	%rax, %rdi
	movb	$32, 48(%rsp)
	leaq	48(%rsp), %rsi
.LEHB15:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
.LEHE15:
	movq	%rax, %rdi
	movq	(%rax), %rax
	movq	%rbx, %rsi
	movq	-24(%rax), %rax
	movq	$8, 16(%rdi,%rax)
.LEHB16:
	call	_ZNSo9_M_insertIlEERSoT_
.LEHE16:
	movq	%rax, %r12
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r12,%rax), %rbx
	testq	%rbx, %rbx
	je	.L398
	cmpb	$0, 56(%rbx)
	je	.L285
	movzbl	67(%rbx), %eax
.L286:
	movsbl	%al, %esi
	movq	%r12, %rdi
.LEHB17:
	call	_ZNSo3putEc
.LEHE17:
	movq	%rax, %rdi
.LEHB18:
	call	_ZNSo5flushEv
.LEHE18:
	movq	176(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L294
	call	_ZdlPv
.L294:
	movq	144(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L386
	call	_ZdlPv
.L386:
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L259:
	.cfi_restore_state
	movq	%rbx, %rdi
.LEHB19:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
.LEHE19:
	movq	(%rbx), %rax
	movl	$10, %esi
	movq	%rbx, %rdi
.LEHB20:
	call	*48(%rax)
.LEHE20:
	jmp	.L260
	.p2align 4,,10
	.p2align 3
.L285:
	movq	%rbx, %rdi
.LEHB21:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
.LEHE21:
	movq	(%rbx), %rax
	movl	$10, %esi
	movq	%rbx, %rdi
.LEHB22:
	call	*48(%rax)
.LEHE22:
	jmp	.L286
	.p2align 4,,10
	.p2align 3
.L393:
	testl	%edx, %edx
	jne	.L399
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	.p2align 4,,5
	jmp	.L207
	.p2align 4,,10
	.p2align 3
.L390:
	testl	%edx, %edx
	jne	.L400
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	jmp	.L184
	.p2align 4,,10
	.p2align 3
.L392:
	leaq	112(%rsp), %rcx
	movq	%r12, %rdx
	leaq	176(%rsp), %rdi
	subq	%r8, %rdx
.LEHB23:
	call	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd
.LEHE23:
	jmp	.L203
	.p2align 4,,10
	.p2align 3
.L389:
	leaq	80(%rsp), %rcx
	movq	%r12, %rdx
	xorl	%esi, %esi
	leaq	144(%rsp), %rdi
.LEHB24:
	call	_ZNSt6vectorIdSaIdEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPdS1_EEmRKd
.LEHE24:
	movq	184(%rsp), %rsi
	movq	176(%rsp), %r13
	movq	%rsi, %r8
	subq	%r13, %r8
	sarq	$3, %r8
	jmp	.L179
	.p2align 4,,10
	.p2align 3
.L305:
	movl	$4, %edx
	jmp	.L185
	.p2align 4,,10
	.p2align 3
.L311:
	movl	$4, %edx
	jmp	.L208
	.p2align 4,,10
	.p2align 3
.L308:
	movl	$1, %edx
	jmp	.L208
	.p2align 4,,10
	.p2align 3
.L309:
	movl	$2, %edx
	jmp	.L208
	.p2align 4,,10
	.p2align 3
.L310:
	movl	$3, %edx
	jmp	.L208
	.p2align 4,,10
	.p2align 3
.L302:
	movl	$1, %edx
	jmp	.L185
	.p2align 4,,10
	.p2align 3
.L303:
	movl	$2, %edx
	jmp	.L185
	.p2align 4,,10
	.p2align 3
.L304:
	movl	$3, %edx
	jmp	.L185
.L312:
	movq	$0, 8(%rsp)
	jmp	.L232
.L314:
.L388:
	movq	176(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L385
	movq	%rax, 8(%rsp)
	vzeroupper
	call	_ZdlPv
	movq	8(%rsp), %rax
.L297:
	movq	144(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L298
	movq	%rax, 8(%rsp)
	call	_ZdlPv
	movq	8(%rsp), %rax
.L298:
	movq	%rax, %rdi
.LEHB25:
	call	_Unwind_Resume
.LEHE25:
.L385:
	vzeroupper
	jmp	.L297
.L320:
	jmp	.L388
.L318:
	.p2align 4,,8
	jmp	.L388
.L317:
	.p2align 4,,11
	jmp	.L388
.L324:
	.p2align 4,,11
	jmp	.L388
.L323:
	.p2align 4,,11
	jmp	.L388
.L322:
	.p2align 4,,11
	jmp	.L388
.L321:
	.p2align 4,,11
	jmp	.L388
.L396:
	.p2align 4,,11
.LEHB26:
	call	_ZSt16__throw_bad_castv
.LEHE26:
.L398:
	.p2align 4,,8
.LEHB27:
	call	_ZSt16__throw_bad_castv
.LEHE27:
.L313:
	.p2align 4,,5
	jmp	.L388
.L325:
	.p2align 4,,8
	jmp	.L388
.L337:
	.p2align 4,,11
	jmp	.L388
.L340:
	.p2align 4,,11
	jmp	.L388
.L336:
	.p2align 4,,11
	jmp	.L388
.L335:
	.p2align 4,,11
	jmp	.L388
.L334:
	.p2align 4,,11
	jmp	.L388
.L333:
	.p2align 4,,11
	jmp	.L388
.L224:
	movl	$_ZZ7dotprodRSt6vectorIdSaIdEES2_E19__PRETTY_FUNCTION__, %ecx
	movl	$19, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L326:
	jmp	.L388
.L397:
	movl	$_ZZ5checkjE19__PRETTY_FUNCTION__, %ecx
	movl	$47, %edx
	movl	$.LC1, %esi
	movl	$.LC16, %edi
	call	__assert_fail
.L331:
	jmp	.L388
.L330:
	jmp	.L388
.L316:
	.p2align 4,,11
	jmp	.L388
.L329:
	.p2align 4,,11
	jmp	.L388
.L328:
	.p2align 4,,11
	jmp	.L388
.L332:
	.p2align 4,,11
	jmp	.L388
.L327:
	.p2align 4,,11
	jmp	.L388
.L338:
	.p2align 4,,11
	jmp	.L388
.L339:
	.p2align 4,,11
	jmp	.L388
.L399:
	movl	%edx, %ecx
	.p2align 4,,9
	jmp	.L206
.L400:
	movl	%edx, %ecx
	.p2align 4,,7
	jmp	.L183
.L315:
	.p2align 4,,7
	jmp	.L388
	.cfi_endproc
.LFE1881:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1881:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1881-.LLSDACSB1881
.LLSDACSB1881:
	.uleb128 .LEHB0-.LFB1881
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L316-.LFB1881
	.uleb128 0
	.uleb128 .LEHB1-.LFB1881
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L317-.LFB1881
	.uleb128 0
	.uleb128 .LEHB2-.LFB1881
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L318-.LFB1881
	.uleb128 0
	.uleb128 .LEHB3-.LFB1881
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L320-.LFB1881
	.uleb128 0
	.uleb128 .LEHB4-.LFB1881
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L321-.LFB1881
	.uleb128 0
	.uleb128 .LEHB5-.LFB1881
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L322-.LFB1881
	.uleb128 0
	.uleb128 .LEHB6-.LFB1881
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L323-.LFB1881
	.uleb128 0
	.uleb128 .LEHB7-.LFB1881
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L324-.LFB1881
	.uleb128 0
	.uleb128 .LEHB8-.LFB1881
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L328-.LFB1881
	.uleb128 0
	.uleb128 .LEHB9-.LFB1881
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L329-.LFB1881
	.uleb128 0
	.uleb128 .LEHB10-.LFB1881
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L330-.LFB1881
	.uleb128 0
	.uleb128 .LEHB11-.LFB1881
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L331-.LFB1881
	.uleb128 0
	.uleb128 .LEHB12-.LFB1881
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L332-.LFB1881
	.uleb128 0
	.uleb128 .LEHB13-.LFB1881
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L333-.LFB1881
	.uleb128 0
	.uleb128 .LEHB14-.LFB1881
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L334-.LFB1881
	.uleb128 0
	.uleb128 .LEHB15-.LFB1881
	.uleb128 .LEHE15-.LEHB15
	.uleb128 .L335-.LFB1881
	.uleb128 0
	.uleb128 .LEHB16-.LFB1881
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L336-.LFB1881
	.uleb128 0
	.uleb128 .LEHB17-.LFB1881
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L340-.LFB1881
	.uleb128 0
	.uleb128 .LEHB18-.LFB1881
	.uleb128 .LEHE18-.LEHB18
	.uleb128 .L313-.LFB1881
	.uleb128 0
	.uleb128 .LEHB19-.LFB1881
	.uleb128 .LEHE19-.LEHB19
	.uleb128 .L326-.LFB1881
	.uleb128 0
	.uleb128 .LEHB20-.LFB1881
	.uleb128 .LEHE20-.LEHB20
	.uleb128 .L327-.LFB1881
	.uleb128 0
	.uleb128 .LEHB21-.LFB1881
	.uleb128 .LEHE21-.LEHB21
	.uleb128 .L338-.LFB1881
	.uleb128 0
	.uleb128 .LEHB22-.LFB1881
	.uleb128 .LEHE22-.LEHB22
	.uleb128 .L339-.LFB1881
	.uleb128 0
	.uleb128 .LEHB23-.LFB1881
	.uleb128 .LEHE23-.LEHB23
	.uleb128 .L315-.LFB1881
	.uleb128 0
	.uleb128 .LEHB24-.LFB1881
	.uleb128 .LEHE24-.LEHB24
	.uleb128 .L314-.LFB1881
	.uleb128 0
	.uleb128 .LEHB25-.LFB1881
	.uleb128 .LEHE25-.LEHB25
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB26-.LFB1881
	.uleb128 .LEHE26-.LEHB26
	.uleb128 .L325-.LFB1881
	.uleb128 0
	.uleb128 .LEHB27-.LFB1881
	.uleb128 .LEHE27-.LEHB27
	.uleb128 .L337-.LFB1881
	.uleb128 0
.LLSDACSE1881:
	.text
	.size	_Z5checkj, .-_Z5checkj
	.p2align 4,,15
	.globl	_Z5test0v
	.type	_Z5test0v, @function
_Z5test0v:
.LFB1882:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$1, %ebx
	.p2align 4,,10
	.p2align 3
.L403:
	movl	%ebx, %edi
	addl	$1, %ebx
	call	_Z5checkj
	cmpl	$100, %ebx
	jne	.L403
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1882:
	.size	_Z5test0v, .-_Z5test0v
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1883:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$1, %ebx
	.p2align 4,,10
	.p2align 3
.L407:
	movl	%ebx, %edi
	addl	$1, %ebx
	call	_Z5checkj
	cmpl	$100, %ebx
	jne	.L407
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1883:
	.size	main, .-main
	.p2align 4,,15
	.type	_GLOBAL__sub_I__Z11fill_vectorRSt6vectorIdSaIdEEj, @function
_GLOBAL__sub_I__Z11fill_vectorRSt6vectorIdSaIdEEj:
.LFB1986:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE1986:
	.size	_GLOBAL__sub_I__Z11fill_vectorRSt6vectorIdSaIdEEj, .-_GLOBAL__sub_I__Z11fill_vectorRSt6vectorIdSaIdEEj
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z11fill_vectorRSt6vectorIdSaIdEEj
	.section	.rodata
	.align 16
	.type	_ZZ5checkjE19__PRETTY_FUNCTION__, @object
	.size	_ZZ5checkjE19__PRETTY_FUNCTION__, 17
_ZZ5checkjE19__PRETTY_FUNCTION__:
	.string	"void check(uint)"
	.align 32
	.type	_ZZ7dotprodRSt6vectorIdSaIdEES2_E19__PRETTY_FUNCTION__, @object
	.size	_ZZ7dotprodRSt6vectorIdSaIdEES2_E19__PRETTY_FUNCTION__, 39
_ZZ7dotprodRSt6vectorIdSaIdEES2_E19__PRETTY_FUNCTION__:
	.string	"double dotprod(double_vt&, double_vt&)"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	0
	.long	1072693248
	.align 8
.LC5:
	.long	0
	.long	1073741824
	.align 8
.LC6:
	.long	0
	.long	1074266112
	.align 8
.LC7:
	.long	0
	.long	1074790400
	.align 8
.LC8:
	.long	0
	.long	1075052544
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC9:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC10:
	.long	1
	.long	1
	.long	1
	.long	1
	.align 16
.LC11:
	.long	0
	.long	1106247680
	.long	0
	.long	1106247680
	.align 16
.LC14:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.section	.rodata.cst8
	.align 8
.LC15:
	.long	1179047063
	.long	1013386997
	.hidden	__dso_handle
	.ident	"GCC: (SUSE Linux) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
