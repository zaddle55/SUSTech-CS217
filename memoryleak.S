	.file	"memoryleak.c"
# GNU C17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata
.LC0:
	.string	"Memory leak test"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$16, %rsp	#,
# memoryleak.c:6:     for (int i = 0; i < 16; i++) {
	movl	$0, -12(%rbp)	#, i
# memoryleak.c:6:     for (int i = 0; i < 16; i++) {
	jmp	.L2	#
.L3:
# memoryleak.c:7:         int* p = (int*)malloc(1024 * 1024 * 1024);
	movl	$1073741824, %edi	#,
	call	malloc@PLT	#
	movq	%rax, -8(%rbp)	# tmp84, p
# memoryleak.c:8:         *p = i;
	movq	-8(%rbp), %rax	# p, tmp85
	movl	-12(%rbp), %edx	# i, tmp86
	movl	%edx, (%rax)	# tmp86, *p_8
# memoryleak.c:6:     for (int i = 0; i < 16; i++) {
	addl	$1, -12(%rbp)	#, i
.L2:
# memoryleak.c:6:     for (int i = 0; i < 16; i++) {
	cmpl	$15, -12(%rbp)	#, i
	jle	.L3	#,
# memoryleak.c:10:     printf("Memory leak test\n");
	leaq	.LC0(%rip), %rax	#, tmp87
	movq	%rax, %rdi	# tmp87,
	call	puts@PLT	#
# memoryleak.c:12:     return 0;
	movl	$0, %eax	#, _6
# memoryleak.c:13: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	main, .-main
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
