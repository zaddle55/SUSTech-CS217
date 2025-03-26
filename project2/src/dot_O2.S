	.file	"dotproduct.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%hhd"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Failed to read int8 type from filestream.\n"
	.text
	.p2align 4
	.globl	vec_fromFileStream_i8
	.type	vec_fromFileStream_i8, @function
vec_fromFileStream_i8:
.LFB51:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	je	.L10
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	leaq	.LC0(%rip), %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	leaq	(%rsi,%rdx), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rsi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	.p2align 4,,10
	.p2align 3
.L4:
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%r13, %rsi
	movq	%r12, %rdi
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	jne	.L14
	addq	$1, %rbx
	cmpq	%rbp, %rbx
	jne	.L4
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
.L14:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movq	stderr(%rip), %rcx
	movl	$42, %edx
	movl	$1, %esi
	leaq	.LC1(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE51:
	.size	vec_fromFileStream_i8, .-vec_fromFileStream_i8
	.section	.rodata.str1.1
.LC2:
	.string	"%hd"
	.section	.rodata.str1.8
	.align 8
.LC3:
	.string	"Failed to read int16 type from filestream.\n"
	.text
	.p2align 4
	.globl	vec_fromFileStream_i16
	.type	vec_fromFileStream_i16, @function
vec_fromFileStream_i16:
.LFB52:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	je	.L24
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leaq	.LC2(%rip), %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %rbx
	.p2align 4,,10
	.p2align 3
.L18:
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%r14, %rsi
	movq	%r12, %rdi
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	jne	.L27
	addq	$1, %rbp
	addq	$2, %rbx
	cmpq	%rbp, %r13
	jne	.L18
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L24:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	ret
.L27:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	stderr(%rip), %rcx
	movl	$43, %edx
	movl	$1, %esi
	leaq	.LC3(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE52:
	.size	vec_fromFileStream_i16, .-vec_fromFileStream_i16
	.section	.rodata.str1.1
.LC4:
	.string	"%d"
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"Failed to read int32 type from filestream.\n"
	.text
	.p2align 4
	.globl	vec_fromFileStream_i32
	.type	vec_fromFileStream_i32, @function
vec_fromFileStream_i32:
.LFB53:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	je	.L37
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leaq	.LC4(%rip), %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %rbx
	.p2align 4,,10
	.p2align 3
.L31:
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%r14, %rsi
	movq	%r12, %rdi
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	jne	.L40
	addq	$1, %rbp
	addq	$4, %rbx
	cmpq	%rbp, %r13
	jne	.L31
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L37:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	ret
.L40:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	stderr(%rip), %rcx
	movl	$43, %edx
	movl	$1, %esi
	leaq	.LC5(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE53:
	.size	vec_fromFileStream_i32, .-vec_fromFileStream_i32
	.section	.rodata.str1.1
.LC6:
	.string	"%f"
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"Failed to read float32 type from filestream.\n"
	.text
	.p2align 4
	.globl	vec_fromFileStream_f32
	.type	vec_fromFileStream_f32, @function
vec_fromFileStream_f32:
.LFB54:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	je	.L50
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leaq	.LC6(%rip), %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %rbx
	.p2align 4,,10
	.p2align 3
.L44:
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%r14, %rsi
	movq	%r12, %rdi
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	jne	.L53
	addq	$1, %rbp
	addq	$4, %rbx
	cmpq	%rbp, %r13
	jne	.L44
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L50:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	ret
.L53:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	stderr(%rip), %rcx
	movl	$45, %edx
	movl	$1, %esi
	leaq	.LC7(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE54:
	.size	vec_fromFileStream_f32, .-vec_fromFileStream_f32
	.section	.rodata.str1.1
.LC8:
	.string	"%lf"
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"Failed to read float64 type from filestream.\n"
	.text
	.p2align 4
	.globl	vec_fromFileStream_f64
	.type	vec_fromFileStream_f64, @function
vec_fromFileStream_f64:
.LFB55:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	je	.L63
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leaq	.LC8(%rip), %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %rbx
	.p2align 4,,10
	.p2align 3
.L57:
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%r14, %rsi
	movq	%r12, %rdi
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	jne	.L66
	addq	$1, %rbp
	addq	$8, %rbx
	cmpq	%rbp, %r13
	jne	.L57
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L63:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	ret
.L66:
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	stderr(%rip), %rcx
	movl	$45, %edx
	movl	$1, %esi
	leaq	.LC9(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE55:
	.size	vec_fromFileStream_f64, .-vec_fromFileStream_f64
	.p2align 4
	.globl	dotproduct_plain_i8
	.type	dotproduct_plain_i8, @function
dotproduct_plain_i8:
.LFB56:
	.cfi_startproc
	endbr64
	testq	%rcx, %rcx
	je	.L67
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L69:
	movzbl	(%rdi,%r8), %eax
	mulb	(%rsi,%r8)
	movb	%al, (%rdx,%r8)
	addq	$1, %r8
	cmpq	%r8, %rcx
	jne	.L69
.L67:
	ret
	.cfi_endproc
.LFE56:
	.size	dotproduct_plain_i8, .-dotproduct_plain_i8
	.p2align 4
	.globl	dotproduct_plain_i16
	.type	dotproduct_plain_i16, @function
dotproduct_plain_i16:
.LFB57:
	.cfi_startproc
	endbr64
	testq	%rcx, %rcx
	je	.L74
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L76:
	movzwl	(%rdi,%rax,2), %r8d
	imulw	(%rsi,%rax,2), %r8w
	movw	%r8w, (%rdx,%rax,2)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L76
.L74:
	ret
	.cfi_endproc
.LFE57:
	.size	dotproduct_plain_i16, .-dotproduct_plain_i16
	.p2align 4
	.globl	dotproduct_plain_i32
	.type	dotproduct_plain_i32, @function
dotproduct_plain_i32:
.LFB58:
	.cfi_startproc
	endbr64
	testq	%rcx, %rcx
	je	.L81
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L83:
	movl	(%rdi,%rax,4), %r8d
	imull	(%rsi,%rax,4), %r8d
	movl	%r8d, (%rdx,%rax,4)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L83
.L81:
	ret
	.cfi_endproc
.LFE58:
	.size	dotproduct_plain_i32, .-dotproduct_plain_i32
	.p2align 4
	.globl	dotproduct_plain_f32
	.type	dotproduct_plain_f32, @function
dotproduct_plain_f32:
.LFB59:
	.cfi_startproc
	endbr64
	testq	%rcx, %rcx
	je	.L88
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L90:
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L90
.L88:
	ret
	.cfi_endproc
.LFE59:
	.size	dotproduct_plain_f32, .-dotproduct_plain_f32
	.p2align 4
	.globl	dotproduct_plain_f64
	.type	dotproduct_plain_f64, @function
dotproduct_plain_f64:
.LFB60:
	.cfi_startproc
	endbr64
	testq	%rcx, %rcx
	je	.L95
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L97:
	movsd	(%rdi,%rax,8), %xmm0
	mulsd	(%rsi,%rax,8), %xmm0
	movsd	%xmm0, (%rdx,%rax,8)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L97
.L95:
	ret
	.cfi_endproc
.LFE60:
	.size	dotproduct_plain_f64, .-dotproduct_plain_f64
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"Failed to allocate memory for vec1\n"
	.align 8
.LC11:
	.string	"Failed to allocate memory for vec2\n"
	.align 8
.LC12:
	.string	"Failed to allocate memory for result\n"
	.section	.rodata.str1.1
.LC14:
	.string	"Dot product time: %f seconds\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB61:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	xorl	%edi, %edi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	call	time@PLT
	movl	%eax, %edi
	call	srand@PLT
	movl	$1, %esi
	movl	$8000000, %edi
	call	calloc@PLT
	testq	%rax, %rax
	je	.L113
	movq	%rax, %rbp
	movq	%rax, %r12
	leaq	8000000(%rax), %rbx
	.p2align 4,,10
	.p2align 3
.L104:
	call	rand@PLT
	addq	$1, %r12
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%r12)
	cmpq	%r12, %rbx
	jne	.L104
	movl	$1, %esi
	movl	$8000000, %edi
	call	calloc@PLT
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L114
	movq	%rax, %r13
	leaq	8000000(%rax), %rbx
	.p2align 4,,10
	.p2align 3
.L106:
	call	rand@PLT
	addq	$1, %r13
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%r13)
	cmpq	%r13, %rbx
	jne	.L106
	movl	$8000000, %edi
	call	malloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L115
	call	clock@PLT
	xorl	%edx, %edx
	movq	%rax, %r13
	.p2align 4,,10
	.p2align 3
.L108:
	movzbl	0(%rbp,%rdx), %eax
	mulb	(%r12,%rdx)
	movb	%al, (%rbx,%rdx)
	addq	$1, %rdx
	cmpq	$8000000, %rdx
	jne	.L108
	call	clock@PLT
	pxor	%xmm0, %xmm0
	movl	$1, %edi
	leaq	.LC14(%rip), %rsi
	subq	%r13, %rax
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %eax
	divsd	.LC13(%rip), %xmm0
	call	__printf_chk@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L113:
	.cfi_restore_state
	movq	stderr(%rip), %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC10(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L115:
	movq	stderr(%rip), %rcx
	movl	$37, %edx
	movl	$1, %esi
	leaq	.LC12(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L114:
	movq	stderr(%rip), %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC11(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE61:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC13:
	.long	0
	.long	1093567616
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
