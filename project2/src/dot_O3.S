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
	movq	%rdi, %r8
	movq	%rsi, %rdi
	movq	%rdx, %rsi
	testq	%rcx, %rcx
	je	.L67
	leaq	1(%rdi), %rdx
	movq	%rsi, %rax
	subq	%rdx, %rax
	cmpq	$14, %rax
	leaq	-1(%rcx), %rax
	seta	%dl
	cmpq	$14, %rax
	seta	%al
	testb	%al, %dl
	je	.L73
	leaq	1(%r8), %rdx
	movq	%rsi, %rax
	subq	%rdx, %rax
	cmpq	$14, %rax
	jbe	.L73
	movq	%rcx, %rdx
	movdqa	.LC10(%rip), %xmm3
	xorl	%eax, %eax
	andq	$-16, %rdx
	.p2align 4,,10
	.p2align 3
.L70:
	movdqu	(%r8,%rax), %xmm2
	movdqu	(%rdi,%rax), %xmm0
	movdqa	%xmm2, %xmm4
	movdqa	%xmm0, %xmm1
	punpcklbw	%xmm0, %xmm1
	punpcklbw	%xmm2, %xmm4
	punpckhbw	%xmm0, %xmm0
	punpckhbw	%xmm2, %xmm2
	pmullw	%xmm4, %xmm1
	pmullw	%xmm2, %xmm0
	pand	%xmm3, %xmm1
	pand	%xmm3, %xmm0
	packuswb	%xmm0, %xmm1
	movups	%xmm1, (%rsi,%rax)
	addq	$16, %rax
	cmpq	%rdx, %rax
	jne	.L70
	movq	%rcx, %rdx
	andq	$-16, %rdx
	testb	$15, %cl
	je	.L67
	movzbl	(%r8,%rdx), %eax
	mulb	(%rdi,%rdx)
	movb	%al, (%rsi,%rdx)
	leaq	1(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	1(%r8,%rdx), %eax
	mulb	1(%rdi,%rdx)
	movb	%al, 1(%rsi,%rdx)
	leaq	2(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	2(%r8,%rdx), %eax
	mulb	2(%rdi,%rdx)
	movb	%al, 2(%rsi,%rdx)
	leaq	3(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	3(%r8,%rdx), %eax
	mulb	3(%rdi,%rdx)
	movb	%al, 3(%rsi,%rdx)
	leaq	4(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	4(%r8,%rdx), %eax
	mulb	4(%rdi,%rdx)
	movb	%al, 4(%rsi,%rdx)
	leaq	5(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	5(%r8,%rdx), %eax
	mulb	5(%rdi,%rdx)
	movb	%al, 5(%rsi,%rdx)
	leaq	6(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	6(%r8,%rdx), %eax
	mulb	6(%rdi,%rdx)
	movb	%al, 6(%rsi,%rdx)
	leaq	7(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	7(%r8,%rdx), %eax
	mulb	7(%rdi,%rdx)
	movb	%al, 7(%rsi,%rdx)
	leaq	8(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	8(%r8,%rdx), %eax
	mulb	8(%rdi,%rdx)
	movb	%al, 8(%rsi,%rdx)
	leaq	9(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	9(%r8,%rdx), %eax
	mulb	9(%rdi,%rdx)
	movb	%al, 9(%rsi,%rdx)
	leaq	10(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	10(%r8,%rdx), %eax
	mulb	10(%rdi,%rdx)
	movb	%al, 10(%rsi,%rdx)
	leaq	11(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	11(%r8,%rdx), %eax
	mulb	11(%rdi,%rdx)
	movb	%al, 11(%rsi,%rdx)
	leaq	12(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	12(%r8,%rdx), %eax
	mulb	12(%rdi,%rdx)
	movb	%al, 12(%rsi,%rdx)
	leaq	13(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	13(%r8,%rdx), %eax
	mulb	13(%rdi,%rdx)
	movb	%al, 13(%rsi,%rdx)
	leaq	14(%rdx), %rax
	cmpq	%rax, %rcx
	jbe	.L67
	movzbl	14(%r8,%rdx), %eax
	mulb	14(%rdi,%rdx)
	movb	%al, 14(%rsi,%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L73:
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L69:
	movzbl	(%r8,%rdx), %eax
	mulb	(%rdi,%rdx)
	movb	%al, (%rsi,%rdx)
	addq	$1, %rdx
	cmpq	%rdx, %rcx
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
	je	.L85
	leaq	2(%rsi), %r9
	movq	%rdx, %r8
	leaq	-1(%rcx), %rax
	subq	%r9, %r8
	cmpq	$12, %r8
	seta	%r9b
	cmpq	$2, %rax
	seta	%r8b
	testb	%r8b, %r9b
	je	.L94
	leaq	2(%rdi), %r9
	movq	%rdx, %r8
	subq	%r9, %r8
	cmpq	$12, %r8
	jbe	.L94
	movq	%rcx, %r8
	cmpq	$6, %rax
	jbe	.L95
	shrq	$3, %r8
	xorl	%eax, %eax
	salq	$4, %r8
	.p2align 4,,10
	.p2align 3
.L89:
	movdqu	(%rdi,%rax), %xmm0
	movdqu	(%rsi,%rax), %xmm2
	pmullw	%xmm2, %xmm0
	movups	%xmm0, (%rdx,%rax)
	addq	$16, %rax
	cmpq	%rax, %r8
	jne	.L89
	movq	%rcx, %rax
	andq	$-8, %rax
	testb	$7, %cl
	je	.L85
	movq	%rcx, %r8
	subq	%rax, %r8
	leaq	-1(%r8), %r9
	cmpq	$2, %r9
	jbe	.L91
.L88:
	movq	(%rdi,%rax,2), %xmm0
	movq	(%rsi,%rax,2), %xmm1
	movq	%r8, %r9
	andq	$-4, %r9
	pmullw	%xmm1, %xmm0
	movq	%xmm0, (%rdx,%rax,2)
	addq	%r9, %rax
	cmpq	%r9, %r8
	je	.L85
.L91:
	movzwl	(%rdi,%rax,2), %r9d
	imulw	(%rsi,%rax,2), %r9w
	leaq	(%rax,%rax), %r8
	movw	%r9w, (%rdx,%rax,2)
	leaq	1(%rax), %r9
	cmpq	%r9, %rcx
	jbe	.L85
	movzwl	2(%rdi,%r8), %r9d
	imulw	2(%rsi,%r8), %r9w
	addq	$2, %rax
	movw	%r9w, 2(%rdx,%r8)
	cmpq	%rax, %rcx
	jbe	.L85
	movzwl	4(%rdi,%r8), %eax
	imulw	4(%rsi,%r8), %ax
	movw	%ax, 4(%rdx,%r8)
	ret
	.p2align 4,,10
	.p2align 3
.L94:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L87:
	movzwl	(%rdi,%rax,2), %r8d
	imulw	(%rsi,%rax,2), %r8w
	movw	%r8w, (%rdx,%rax,2)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L87
.L85:
	ret
.L95:
	xorl	%eax, %eax
	jmp	.L88
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
	je	.L107
	leaq	4(%rsi), %r8
	movq	%rdx, %rax
	subq	%r8, %rax
	cmpq	$8, %rax
	leaq	-1(%rcx), %rax
	seta	%r8b
	cmpq	$2, %rax
	seta	%al
	testb	%al, %r8b
	je	.L113
	leaq	4(%rdi), %r8
	movq	%rdx, %rax
	subq	%r8, %rax
	cmpq	$8, %rax
	jbe	.L113
	movq	%rcx, %r8
	xorl	%eax, %eax
	shrq	$2, %r8
	salq	$4, %r8
	.p2align 4,,10
	.p2align 3
.L110:
	movdqu	(%rdi,%rax), %xmm0
	movdqu	(%rsi,%rax), %xmm2
	movdqu	(%rdi,%rax), %xmm1
	movdqu	(%rsi,%rax), %xmm3
	psrlq	$32, %xmm0
	psrlq	$32, %xmm2
	pmuludq	%xmm3, %xmm1
	pmuludq	%xmm2, %xmm0
	pshufd	$8, %xmm1, %xmm1
	pshufd	$8, %xmm0, %xmm0
	punpckldq	%xmm0, %xmm1
	movups	%xmm1, (%rdx,%rax)
	addq	$16, %rax
	cmpq	%r8, %rax
	jne	.L110
	movq	%rcx, %rax
	andq	$-4, %rax
	testb	$3, %cl
	je	.L107
	movl	(%rdi,%rax,4), %r9d
	imull	(%rsi,%rax,4), %r9d
	leaq	0(,%rax,4), %r8
	movl	%r9d, (%rdx,%rax,4)
	leaq	1(%rax), %r9
	cmpq	%r9, %rcx
	jbe	.L107
	movl	4(%rdi,%r8), %r9d
	imull	4(%rsi,%r8), %r9d
	addq	$2, %rax
	movl	%r9d, 4(%rdx,%r8)
	cmpq	%rax, %rcx
	jbe	.L107
	movl	8(%rdi,%r8), %eax
	imull	8(%rsi,%r8), %eax
	movl	%eax, 8(%rdx,%r8)
	ret
	.p2align 4,,10
	.p2align 3
.L113:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L109:
	movl	(%rdi,%rax,4), %r8d
	imull	(%rsi,%rax,4), %r8d
	movl	%r8d, (%rdx,%rax,4)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L109
.L107:
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
	je	.L125
	leaq	4(%rsi), %r8
	movq	%rdx, %rax
	subq	%r8, %rax
	cmpq	$8, %rax
	seta	%r8b
	cmpq	$1, %rcx
	setne	%al
	testb	%al, %r8b
	je	.L134
	leaq	4(%rdi), %r8
	movq	%rdx, %rax
	subq	%r8, %rax
	cmpq	$8, %rax
	jbe	.L134
	leaq	-1(%rcx), %rax
	cmpq	$2, %rax
	jbe	.L135
	movq	%rcx, %r8
	xorl	%eax, %eax
	shrq	$2, %r8
	salq	$4, %r8
	.p2align 4,,10
	.p2align 3
.L129:
	movups	(%rdi,%rax), %xmm0
	movups	(%rsi,%rax), %xmm2
	mulps	%xmm2, %xmm0
	movups	%xmm0, (%rdx,%rax)
	addq	$16, %rax
	cmpq	%r8, %rax
	jne	.L129
	movq	%rcx, %rax
	andq	$-4, %rax
	testb	$3, %cl
	je	.L125
	subq	%rax, %rcx
	cmpq	$1, %rcx
	je	.L131
.L128:
	movq	(%rdi,%rax,4), %xmm0
	movq	(%rsi,%rax,4), %xmm1
	movq	%rcx, %r8
	andq	$-2, %r8
	mulps	%xmm1, %xmm0
	movlps	%xmm0, (%rdx,%rax,4)
	addq	%r8, %rax
	cmpq	%r8, %rcx
	je	.L125
.L131:
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	ret
	.p2align 4,,10
	.p2align 3
.L134:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L127:
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L127
.L125:
	ret
.L135:
	xorl	%eax, %eax
	jmp	.L128
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
	movq	%rsi, %r8
	testq	%rcx, %rcx
	je	.L150
	leaq	8(%rsi), %rax
	cmpq	%rax, %rdx
	leaq	8(%rdi), %rax
	setne	%sil
	cmpq	%rax, %rdx
	setne	%al
	testb	%al, %sil
	je	.L156
	leaq	-1(%rcx), %rax
	cmpq	$1, %rax
	jbe	.L156
	movq	%rcx, %rsi
	xorl	%eax, %eax
	shrq	%rsi
	salq	$4, %rsi
	.p2align 4,,10
	.p2align 3
.L153:
	movupd	(%rdi,%rax), %xmm0
	movupd	(%r8,%rax), %xmm1
	mulpd	%xmm1, %xmm0
	movups	%xmm0, (%rdx,%rax)
	addq	$16, %rax
	cmpq	%rax, %rsi
	jne	.L153
	movq	%rcx, %rax
	andq	$-2, %rax
	andl	$1, %ecx
	je	.L150
	movsd	(%rdi,%rax,8), %xmm0
	mulsd	(%r8,%rax,8), %xmm0
	movsd	%xmm0, (%rdx,%rax,8)
	ret
	.p2align 4,,10
	.p2align 3
.L156:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L152:
	movsd	(%rdi,%rax,8), %xmm0
	mulsd	(%r8,%rax,8), %xmm0
	movsd	%xmm0, (%rdx,%rax,8)
	addq	$1, %rax
	cmpq	%rax, %rcx
	jne	.L152
.L150:
	ret
	.cfi_endproc
.LFE60:
	.size	dotproduct_plain_f64, .-dotproduct_plain_f64
	.section	.rodata.str1.8
	.align 8
.LC11:
	.string	"Failed to allocate memory for vec1\n"
	.align 8
.LC12:
	.string	"Failed to allocate memory for vec2\n"
	.align 8
.LC13:
	.string	"Failed to allocate memory for result\n"
	.section	.rodata.str1.1
.LC15:
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
	je	.L179
	movq	%rax, %rbp
	movq	%rax, %r12
	leaq	8000000(%rax), %rbx
	.p2align 4,,10
	.p2align 3
.L170:
	call	rand@PLT
	addq	$1, %r12
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%r12)
	cmpq	%rbx, %r12
	jne	.L170
	movl	$1, %esi
	movl	$8000000, %edi
	call	calloc@PLT
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L180
	movq	%rax, %r13
	leaq	8000000(%rax), %rbx
	.p2align 4,,10
	.p2align 3
.L172:
	call	rand@PLT
	addq	$1, %r13
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%r13)
	cmpq	%rbx, %r13
	jne	.L172
	movl	$8000000, %edi
	call	malloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L181
	call	clock@PLT
	movdqa	.LC10(%rip), %xmm3
	movq	%rax, %r13
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L174:
	movdqu	(%r12,%rax), %xmm2
	movdqu	0(%rbp,%rax), %xmm0
	movdqa	%xmm2, %xmm4
	movdqa	%xmm0, %xmm1
	punpcklbw	%xmm0, %xmm1
	punpcklbw	%xmm2, %xmm4
	punpckhbw	%xmm0, %xmm0
	punpckhbw	%xmm2, %xmm2
	pmullw	%xmm4, %xmm1
	pmullw	%xmm2, %xmm0
	pand	%xmm3, %xmm1
	pand	%xmm3, %xmm0
	packuswb	%xmm0, %xmm1
	movups	%xmm1, (%rbx,%rax)
	addq	$16, %rax
	cmpq	$8000000, %rax
	jne	.L174
	call	clock@PLT
	pxor	%xmm0, %xmm0
	movl	$1, %edi
	leaq	.LC15(%rip), %rsi
	subq	%r13, %rax
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %eax
	divsd	.LC14(%rip), %xmm0
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
.L179:
	.cfi_restore_state
	movq	stderr(%rip), %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC11(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L181:
	movq	stderr(%rip), %rcx
	movl	$37, %edx
	movl	$1, %esi
	leaq	.LC13(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L180:
	movq	stderr(%rip), %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC12(%rip), %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE61:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC10:
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC14:
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
