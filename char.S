	.file	"char.cpp"
# GNU C++17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata
.LC0:
	.string	":"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1731:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$16, %rsp	#,
# char.cpp:6:     char c1 = 'C';
	movb	$67, -8(%rbp)	#, c1
# char.cpp:7:     char c2 = 0x37; // '7' in ASCII
	movb	$55, -7(%rbp)	#, c2
# char.cpp:9:     char16_t c3 = u'C'; // unicode
	movw	$67, -6(%rbp)	#, c3
# char.cpp:10:     char32_t c4 = U'开'; // unicode
	movl	$24320, -4(%rbp)	#, c4
# char.cpp:12:     std::cout << c1 << ":" << c2 << ":" << c3 << std::endl;
	movsbl	-8(%rbp), %eax	# c1, _1
	movl	%eax, %esi	# _1,
	leaq	_ZSt4cout(%rip), %rax	#, tmp101
	movq	%rax, %rdi	# tmp101,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@PLT	#
	movq	%rax, %rdx	#, _2
	leaq	.LC0(%rip), %rax	#, tmp102
	movq	%rax, %rsi	# tmp102,
	movq	%rdx, %rdi	# _2,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT	#
	movq	%rax, %rdx	#, _3
	movsbl	-7(%rbp), %eax	# c2, _4
	movl	%eax, %esi	# _4,
	movq	%rdx, %rdi	# _3,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@PLT	#
	movq	%rax, %rdx	#, _5
	leaq	.LC0(%rip), %rax	#, tmp103
	movq	%rax, %rsi	# tmp103,
	movq	%rdx, %rdi	# _5,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT	#
	movq	%rax, %rdx	#, _6
# char.cpp:12:     std::cout << c1 << ":" << c2 << ":" << c3 << std::endl;
	movzwl	-6(%rbp), %eax	# c3, _7
	movl	%eax, %esi	# _7,
	movq	%rdx, %rdi	# _6,
	call	_ZNSolsEi@PLT	#
# char.cpp:12:     std::cout << c1 << ":" << c2 << ":" << c3 << std::endl;
	movq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rdx	#, tmp105
	movq	%rdx, %rsi	# tmp104,
	movq	%rax, %rdi	# _8,
	call	_ZNSolsEPFRSoS_E@PLT	#
# char.cpp:13:     std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
	movsbl	-8(%rbp), %eax	# c1, _9
	movl	%eax, %esi	# _9,
	leaq	_ZSt4cout(%rip), %rax	#, tmp106
	movq	%rax, %rdi	# tmp106,
	call	_ZNSolsEi@PLT	#
	movq	%rax, %rdx	#, _10
	leaq	.LC0(%rip), %rax	#, tmp107
	movq	%rax, %rsi	# tmp107,
	movq	%rdx, %rdi	# _10,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT	#
	movq	%rax, %rdx	#, _11
# char.cpp:13:     std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
	movsbl	-7(%rbp), %eax	# c2, _12
	movl	%eax, %esi	# _12,
	movq	%rdx, %rdi	# _11,
	call	_ZNSolsEi@PLT	#
	movq	%rax, %rdx	#, _13
	leaq	.LC0(%rip), %rax	#, tmp108
	movq	%rax, %rsi	# tmp108,
	movq	%rdx, %rdi	# _13,
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@PLT	#
	movq	%rax, %rdx	#, _14
# char.cpp:13:     std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
	movzwl	-6(%rbp), %eax	# c3, _15
	movl	%eax, %esi	# _15,
	movq	%rdx, %rdi	# _14,
	call	_ZNSolsEi@PLT	#
# char.cpp:13:     std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
	movq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rdx	#, tmp110
	movq	%rdx, %rsi	# tmp109,
	movq	%rax, %rdi	# _16,
	call	_ZNSolsEPFRSoS_E@PLT	#
# char.cpp:14:     std::cout << c4 << std::endl;
	movl	-4(%rbp), %eax	# c4, tmp111
	movl	%eax, %esi	# tmp111,
	leaq	_ZSt4cout(%rip), %rax	#, tmp112
	movq	%rax, %rdi	# tmp112,
	call	_ZNSolsEj@PLT	#
# char.cpp:14:     std::cout << c4 << std::endl;
	movq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rdx	#, tmp114
	movq	%rdx, %rsi	# tmp113,
	movq	%rax, %rdi	# _17,
	call	_ZNSolsEPFRSoS_E@PLT	#
# char.cpp:15: }
	movl	$0, %eax	#, _48
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE1731:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB2236:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$16, %rsp	#,
	movl	%edi, -4(%rbp)	# __initialize_p, __initialize_p
	movl	%esi, -8(%rbp)	# __priority, __priority
# char.cpp:15: }
	cmpl	$1, -4(%rbp)	#, __initialize_p
	jne	.L5	#,
# char.cpp:15: }
	cmpl	$65535, -8(%rbp)	#, __priority
	jne	.L5	#,
# /usr/include/c++/11/iostream:74:   static ios_base::Init __ioinit;
	leaq	_ZStL8__ioinit(%rip), %rax	#, tmp82
	movq	%rax, %rdi	# tmp82,
	call	_ZNSt8ios_base4InitC1Ev@PLT	#
	leaq	__dso_handle(%rip), %rax	#, tmp83
	movq	%rax, %rdx	# tmp83,
	leaq	_ZStL8__ioinit(%rip), %rax	#, tmp84
	movq	%rax, %rsi	# tmp84,
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax	#, tmp86
	movq	%rax, %rdi	# tmp85,
	call	__cxa_atexit@PLT	#
.L5:
# char.cpp:15: }
	nop	
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE2236:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB2237:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
# char.cpp:15: }
	movl	$65535, %esi	#,
	movl	$1, %edi	#,
	call	_Z41__static_initialization_and_destruction_0ii	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE2237:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
