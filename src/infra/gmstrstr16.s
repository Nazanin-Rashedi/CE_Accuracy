	.file	"gmstrstr16.cc"
	.text
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1509:
	pushl	%ebp
.LCFI0:
	movl	%esp, %ebp
.LCFI1:
	subl	$24, %esp
.LCFI2:
	cmpl	$1, 8(%ebp)
	jne	.L3
	cmpl	$65535, 12(%ebp)
	jne	.L3
	movl	$_ZStL8__ioinit, (%esp)
	call	_ZNSt8ios_base4InitC1Ev
	movl	$_ZNSt8ios_base4InitD1Ev, %eax
	movl	$__dso_handle, 8(%esp)
	movl	$_ZStL8__ioinit, 4(%esp)
	movl	%eax, (%esp)
	call	__cxa_atexit
.L3:
	leave
	ret
.LFE1509:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__I_gmstrstr16, @function
_GLOBAL__I_gmstrstr16:
.LFB1510:
	pushl	%ebp
.LCFI3:
	movl	%esp, %ebp
.LCFI4:
	subl	$8, %esp
.LCFI5:
	movl	$65535, 4(%esp)
	movl	$1, (%esp)
	call	_Z41__static_initialization_and_destruction_0ii
	leave
	ret
.LFE1510:
	.size	_GLOBAL__I_gmstrstr16, .-_GLOBAL__I_gmstrstr16
	.section	.ctors,"aw",@progbits
	.align 4
	.long	_GLOBAL__I_gmstrstr16
	.section	.rodata
.LC0:
	.string	"compare."
.LC1:
	.string	"index: "
	.text
.globl gmstrstr16
	.type	gmstrstr16, @function
gmstrstr16:
.LFB1500:
	pushl	%ebp
.LCFI6:
	movl	%esp, %ebp
.LCFI7:
	pushl	%edi
.LCFI8:
	pushl	%esi
.LCFI9:
	pushl	%ebx
.LCFI10:
	subl	$268, %esp
.LCFI11:
	movl	8(%ebp), %eax
	movl	%eax, -160(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -156(%ebp)
	movl	-156(%ebp), %eax
	movl	%eax, -72(%ebp)
	movl	-72(%ebp), %eax
	movdqu	(%eax), %xmm0
	movdqa	%xmm0, -152(%ebp)
	movl	-156(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	movb	%al, -66(%ebp)
	movsbl	-66(%ebp),%edi
	movsbl	-66(%ebp),%eax
	movl	%eax, -212(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -216(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -220(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -224(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -228(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -232(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -236(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -240(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -244(%ebp)
	movsbl	-66(%ebp),%eax
	movl	%eax, -248(%ebp)
	movsbl	-66(%ebp),%ebx
	movsbl	-66(%ebp),%esi
	movsbl	-66(%ebp),%ecx
	movsbl	-66(%ebp),%edx
	movsbl	-66(%ebp),%eax
	movb	%al, -50(%ebp)
	movb	%dl, -51(%ebp)
	movb	%cl, -52(%ebp)
	movl	%esi, %eax
	movb	%al, -53(%ebp)
	movb	%bl, -54(%ebp)
	movzbl	-248(%ebp), %eax
	movb	%al, -55(%ebp)
	movzbl	-244(%ebp), %eax
	movb	%al, -56(%ebp)
	movzbl	-240(%ebp), %eax
	movb	%al, -57(%ebp)
	movzbl	-236(%ebp), %eax
	movb	%al, -58(%ebp)
	movzbl	-232(%ebp), %eax
	movb	%al, -59(%ebp)
	movzbl	-228(%ebp), %eax
	movb	%al, -60(%ebp)
	movzbl	-224(%ebp), %eax
	movb	%al, -61(%ebp)
	movzbl	-220(%ebp), %eax
	movb	%al, -62(%ebp)
	movzbl	-216(%ebp), %eax
	movb	%al, -63(%ebp)
	movzbl	-212(%ebp), %eax
	movb	%al, -64(%ebp)
	movl	%edi, %eax
	movb	%al, -65(%ebp)
	movzbl	-62(%ebp), %ebx
	movzbl	-63(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-64(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-65(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-58(%ebp), %ecx
	movzbl	-59(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-60(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-61(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-54(%ebp), %edx
	movzbl	-55(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-56(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-57(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-50(%ebp), %eax
	movzbl	-51(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movzbl	-52(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movzbl	-53(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movd	%ebx, %xmm0
	movd	%ecx, %xmm2
	punpckldq	%xmm2, %xmm0
	movq	%xmm0, %xmm1
	movd	%edx, %xmm3
	movd	%eax, %xmm2
	punpckldq	%xmm2, %xmm3
	movq	%xmm3, %xmm0
	movq	%xmm1, %xmm3
	punpcklqdq	%xmm0, %xmm3
	movdqa	%xmm3, %xmm0
	movdqa	%xmm0, -136(%ebp)
	movb	$0, -49(%ebp)
	movsbl	-49(%ebp),%edi
	movsbl	-49(%ebp),%eax
	movl	%eax, -172(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -176(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -180(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -184(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -188(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -192(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -196(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -200(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -204(%ebp)
	movsbl	-49(%ebp),%eax
	movl	%eax, -208(%ebp)
	movsbl	-49(%ebp),%ebx
	movsbl	-49(%ebp),%esi
	movsbl	-49(%ebp),%ecx
	movsbl	-49(%ebp),%edx
	movsbl	-49(%ebp),%eax
	movb	%al, -33(%ebp)
	movb	%dl, -34(%ebp)
	movb	%cl, -35(%ebp)
	movl	%esi, %eax
	movb	%al, -36(%ebp)
	movb	%bl, -37(%ebp)
	movzbl	-208(%ebp), %eax
	movb	%al, -38(%ebp)
	movzbl	-204(%ebp), %eax
	movb	%al, -39(%ebp)
	movzbl	-200(%ebp), %eax
	movb	%al, -40(%ebp)
	movzbl	-196(%ebp), %eax
	movb	%al, -41(%ebp)
	movzbl	-192(%ebp), %eax
	movb	%al, -42(%ebp)
	movzbl	-188(%ebp), %eax
	movb	%al, -43(%ebp)
	movzbl	-184(%ebp), %eax
	movb	%al, -44(%ebp)
	movzbl	-180(%ebp), %eax
	movb	%al, -45(%ebp)
	movzbl	-176(%ebp), %eax
	movb	%al, -46(%ebp)
	movzbl	-172(%ebp), %eax
	movb	%al, -47(%ebp)
	movl	%edi, %eax
	movb	%al, -48(%ebp)
	movzbl	-45(%ebp), %ebx
	movzbl	-46(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-47(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-48(%ebp), %eax
	sall	$8, %ebx
	orl	%eax, %ebx
	movzbl	-41(%ebp), %ecx
	movzbl	-42(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-43(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-44(%ebp), %eax
	sall	$8, %ecx
	orl	%eax, %ecx
	movzbl	-37(%ebp), %edx
	movzbl	-38(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-39(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-40(%ebp), %eax
	sall	$8, %edx
	orl	%eax, %edx
	movzbl	-33(%ebp), %eax
	movzbl	-34(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movzbl	-35(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movzbl	-36(%ebp), %esi
	sall	$8, %eax
	orl	%esi, %eax
	movd	%ebx, %xmm0
	movd	%ecx, %xmm2
	punpckldq	%xmm2, %xmm0
	movq	%xmm0, %xmm1
	movd	%edx, %xmm3
	movd	%eax, %xmm2
	punpckldq	%xmm2, %xmm3
	movq	%xmm3, %xmm0
	movq	%xmm1, %xmm3
	punpcklqdq	%xmm0, %xmm3
	movdqa	%xmm3, %xmm0
	movdqa	%xmm0, -120(%ebp)
	movl	$-1, -92(%ebp)
	movl	-160(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	movdqu	(%eax), %xmm0
	movdqa	%xmm0, -88(%ebp)
	movl	$.LC0, 4(%esp)
	movl	$_ZSt4cout, (%esp)
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, 4(%esp)
	movl	%eax, (%esp)
	call	_ZNSolsEPFRSoS_E
	movdqa	-136(%ebp), %xmm1
	movl	-160(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	movdqu	(%eax), %xmm0
	movl	%ecx, -92(%ebp)
	movl	$.LC1, 4(%esp)
	movl	$_ZSt4cout, (%esp)
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	%eax, %edx
	movl	-92(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_ZNSolsEi
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, 4(%esp)
	movl	%eax, (%esp)
	call	_ZNSolsEPFRSoS_E
	movl	-160(%ebp), %eax
	addl	$268, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.LFE1500:
	.size	gmstrstr16, .-gmstrstr16
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.weakref	_ZL20__gthrw_pthread_oncePiPFvvE,pthread_once
	.weakref	_ZL27__gthrw_pthread_getspecificj,pthread_getspecific
	.weakref	_ZL27__gthrw_pthread_setspecificjPKv,pthread_setspecific
	.weakref	_ZL22__gthrw_pthread_createPmPK14pthread_attr_tPFPvS3_ES3_,pthread_create
	.weakref	_ZL22__gthrw_pthread_cancelm,pthread_cancel
	.weakref	_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t,pthread_mutex_lock
	.weakref	_ZL29__gthrw_pthread_mutex_trylockP15pthread_mutex_t,pthread_mutex_trylock
	.weakref	_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t,pthread_mutex_unlock
	.weakref	_ZL26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t,pthread_mutex_init
	.weakref	_ZL30__gthrw_pthread_cond_broadcastP14pthread_cond_t,pthread_cond_broadcast
	.weakref	_ZL25__gthrw_pthread_cond_waitP14pthread_cond_tP15pthread_mutex_t,pthread_cond_wait
	.weakref	_ZL26__gthrw_pthread_key_createPjPFvPvE,pthread_key_create
	.weakref	_ZL26__gthrw_pthread_key_deletej,pthread_key_delete
	.weakref	_ZL30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t,pthread_mutexattr_init
	.weakref	_ZL33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti,pthread_mutexattr_settype
	.weakref	_ZL33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t,pthread_mutexattr_destroy
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
.globl __gxx_personality_v0
	.string	"zP"
	.uleb128 0x1
	.sleb128 -4
	.byte	0x8
	.uleb128 0x5
	.byte	0x0
	.long	__gxx_personality_v0
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x88
	.uleb128 0x1
	.align 4
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB1509
	.long	.LFE1509-.LFB1509
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB1509
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x5
	.align 4
.LEFDE1:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB1500
	.long	.LFE1500-.LFB1500
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB1500
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI11-.LCFI7
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x87
	.uleb128 0x3
	.align 4
.LEFDE5:
	.ident	"GCC: (SUSE Linux) 4.3.2 [gcc-4_3-branch revision 141291]"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.ascii	"ospwg"
	.section	.note.GNU-stack,"",@progbits
