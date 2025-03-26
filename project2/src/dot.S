	.file	"dotproduct.c"
	.text
	.section	.rodata
.LC0:
	.string	"%hhd"
	.align 8
.LC1:
	.string	"Failed to read int8 type from filestream.\n"
	.text
	.globl	vec_fromFileStream_i8
	.type	vec_fromFileStream_i8, @function
vec_fromFileStream_i8:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L4:
	movq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	leaq	.LC0(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	je	.L3
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$42, %edx
	movl	$1, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L3:
	addq	$1, -8(%rbp)
.L2:
	movq	-8(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jb	.L4
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	vec_fromFileStream_i8, .-vec_fromFileStream_i8
	.section	.rodata
.LC2:
	.string	"%hd"
	.align 8
.LC3:
	.string	"Failed to read int16 type from filestream.\n"
	.text
	.globl	vec_fromFileStream_i16
	.type	vec_fromFileStream_i16, @function
vec_fromFileStream_i16:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L6
.L8:
	movq	-8(%rbp), %rax
	leaq	(%rax,%rax), %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	leaq	.LC2(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	je	.L7
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$43, %edx
	movl	$1, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L7:
	addq	$1, -8(%rbp)
.L6:
	movq	-8(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jb	.L8
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	vec_fromFileStream_i16, .-vec_fromFileStream_i16
	.section	.rodata
.LC4:
	.string	"%d"
	.align 8
.LC5:
	.string	"Failed to read int32 type from filestream.\n"
	.text
	.globl	vec_fromFileStream_i32
	.type	vec_fromFileStream_i32, @function
vec_fromFileStream_i32:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L10
.L12:
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	leaq	.LC4(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	je	.L11
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$43, %edx
	movl	$1, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L11:
	addq	$1, -8(%rbp)
.L10:
	movq	-8(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jb	.L12
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	vec_fromFileStream_i32, .-vec_fromFileStream_i32
	.section	.rodata
.LC6:
	.string	"%f"
	.align 8
.LC7:
	.string	"Failed to read float32 type from filestream.\n"
	.text
	.globl	vec_fromFileStream_f32
	.type	vec_fromFileStream_f32, @function
vec_fromFileStream_f32:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L14
.L16:
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	leaq	.LC6(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	je	.L15
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$45, %edx
	movl	$1, %esi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L15:
	addq	$1, -8(%rbp)
.L14:
	movq	-8(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jb	.L16
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	vec_fromFileStream_f32, .-vec_fromFileStream_f32
	.section	.rodata
.LC8:
	.string	"%lf"
	.align 8
.LC9:
	.string	"Failed to read float64 type from filestream.\n"
	.text
	.globl	vec_fromFileStream_f64
	.type	vec_fromFileStream_f64, @function
vec_fromFileStream_f64:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L18
.L20:
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	leaq	.LC8(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_fscanf@PLT
	cmpl	$1, %eax
	je	.L19
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$45, %edx
	movl	$1, %esi
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L19:
	addq	$1, -8(%rbp)
.L18:
	movq	-8(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jb	.L20
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	vec_fromFileStream_f64, .-vec_fromFileStream_f64
	.globl	dotproduct_plain_i8
	.type	dotproduct_plain_i8, @function
dotproduct_plain_i8:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L22
.L23:
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	movl	%eax, %esi
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movl	%eax, %ecx
	movq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movl	%eax, %edx
	movl	%ecx, %eax
	imull	%edx, %eax
	addl	%esi, %eax
	movl	%eax, %edx
	movq	-40(%rbp), %rax
	movb	%dl, (%rax)
	addq	$1, -8(%rbp)
.L22:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L23
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	dotproduct_plain_i8, .-dotproduct_plain_i8
	.globl	dotproduct_plain_i16
	.type	dotproduct_plain_i16, @function
dotproduct_plain_i16:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L25
.L26:
	movq	-40(%rbp), %rax
	movzwl	(%rax), %eax
	movl	%eax, %esi
	movq	-8(%rbp), %rax
	leaq	(%rax,%rax), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movl	%eax, %edx
	movq	-8(%rbp), %rax
	leaq	(%rax,%rax), %rcx
	movq	-32(%rbp), %rax
	addq	%rcx, %rax
	movzwl	(%rax), %eax
	imull	%edx, %eax
	addl	%esi, %eax
	movl	%eax, %edx
	movq	-40(%rbp), %rax
	movw	%dx, (%rax)
	addq	$1, -8(%rbp)
.L25:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L26
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	dotproduct_plain_i16, .-dotproduct_plain_i16
	.globl	dotproduct_plain_i32
	.type	dotproduct_plain_i32, @function
dotproduct_plain_i32:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L28
.L29:
	movq	-40(%rbp), %rax
	movl	(%rax), %ecx
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rsi
	movq	-32(%rbp), %rax
	addq	%rsi, %rax
	movl	(%rax), %eax
	imull	%edx, %eax
	leal	(%rcx,%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	addq	$1, -8(%rbp)
.L28:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L29
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	dotproduct_plain_i32, .-dotproduct_plain_i32
	.globl	dotproduct_plain_f32
	.type	dotproduct_plain_f32, @function
dotproduct_plain_f32:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L31
.L32:
	movq	-40(%rbp), %rax
	movss	(%rax), %xmm1
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm2
	movq	-8(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	mulss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	movq	-40(%rbp), %rax
	movss	%xmm0, (%rax)
	addq	$1, -8(%rbp)
.L31:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L32
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	dotproduct_plain_f32, .-dotproduct_plain_f32
	.globl	dotproduct_plain_f64
	.type	dotproduct_plain_f64, @function
dotproduct_plain_f64:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L34
.L35:
	movq	-40(%rbp), %rax
	movsd	(%rax), %xmm1
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm2
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	mulsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movq	-40(%rbp), %rax
	movsd	%xmm0, (%rax)
	addq	$1, -8(%rbp)
.L34:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L35
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	dotproduct_plain_f64, .-dotproduct_plain_f64
	.section	.rodata
	.align 8
.LC10:
	.string	"Failed to allocate memory for vec1\n"
	.align 8
.LC11:
	.string	"Failed to allocate memory for vec2\n"
.LC13:
	.string	"Dot product time: %f seconds\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, %edi
	call	time@PLT
	movl	%eax, %edi
	call	srand@PLT
	movq	$8000000, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -48(%rbp)
	cmpq	$0, -48(%rbp)
	jne	.L37
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L37:
	movq	-56(%rbp), %rdx
	movq	-48(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	$0, -72(%rbp)
	jmp	.L38
.L39:
	call	rand@PLT
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movl	%eax, %ecx
	movq	-48(%rbp), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addq	$1, -72(%rbp)
.L38:
	movq	-72(%rbp), %rax
	cmpq	-56(%rbp), %rax
	jb	.L39
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L40
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$35, %edx
	movl	$1, %esi
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L40:
	movq	-56(%rbp), %rdx
	movq	-40(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	$0, -64(%rbp)
	jmp	.L41
.L42:
	call	rand@PLT
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movl	%eax, %ecx
	movq	-40(%rbp), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addq	$1, -64(%rbp)
.L41:
	movq	-64(%rbp), %rax
	cmpq	-56(%rbp), %rax
	jb	.L42
	movb	$0, -73(%rbp)
	call	clock@PLT
	movq	%rax, -32(%rbp)
	movq	-56(%rbp), %rcx
	leaq	-73(%rbp), %rdx
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	dotproduct_plain_i8
	call	clock@PLT
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	subq	-32(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC12(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %xmm0
	leaq	.LC13(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L44
	call	__stack_chk_fail@PLT
.L44:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC12:
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
