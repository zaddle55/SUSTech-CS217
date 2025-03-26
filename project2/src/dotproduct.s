	.file	"dotproduct.c"
	.option pic
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"%hhd"
	.align	3
.LC1:
	.string	"Failed to read int8 type from filestream.\n"
	.text
	.align	1
	.globl	vec_fromFileStream_i8
	.type	vec_fromFileStream_i8, @function
vec_fromFileStream_i8:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	zero,-24(s0)
	j	.L2
.L4:
	ld	a4,-48(s0)
	ld	a5,-24(s0)
	add	a5,a4,a5
	mv	a2,a5
	lla	a1,.LC0
	ld	a0,-40(s0)
	call	__isoc99_fscanf@plt
	mv	a5,a0
	mv	a4,a5
	li	a5,1
	beq	a4,a5,.L3
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,42
	li	a1,1
	lla	a0,.LC1
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L3:
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L2:
	ld	a4,-24(s0)
	ld	a5,-56(s0)
	bltu	a4,a5,.L4
	nop
	nop
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	vec_fromFileStream_i8, .-vec_fromFileStream_i8
	.section	.rodata
	.align	3
.LC2:
	.string	"%hd"
	.align	3
.LC3:
	.string	"Failed to read int16 type from filestream.\n"
	.text
	.align	1
	.globl	vec_fromFileStream_i16
	.type	vec_fromFileStream_i16, @function
vec_fromFileStream_i16:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	zero,-24(s0)
	j	.L6
.L8:
	ld	a5,-24(s0)
	slli	a5,a5,1
	ld	a4,-48(s0)
	add	a5,a4,a5
	mv	a2,a5
	lla	a1,.LC2
	ld	a0,-40(s0)
	call	__isoc99_fscanf@plt
	mv	a5,a0
	mv	a4,a5
	li	a5,1
	beq	a4,a5,.L7
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,43
	li	a1,1
	lla	a0,.LC3
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L7:
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L6:
	ld	a4,-24(s0)
	ld	a5,-56(s0)
	bltu	a4,a5,.L8
	nop
	nop
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	vec_fromFileStream_i16, .-vec_fromFileStream_i16
	.section	.rodata
	.align	3
.LC4:
	.string	"%d"
	.align	3
.LC5:
	.string	"Failed to read int32 type from filestream.\n"
	.text
	.align	1
	.globl	vec_fromFileStream_i32
	.type	vec_fromFileStream_i32, @function
vec_fromFileStream_i32:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	zero,-24(s0)
	j	.L10
.L12:
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	mv	a2,a5
	lla	a1,.LC4
	ld	a0,-40(s0)
	call	__isoc99_fscanf@plt
	mv	a5,a0
	mv	a4,a5
	li	a5,1
	beq	a4,a5,.L11
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,43
	li	a1,1
	lla	a0,.LC5
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L11:
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L10:
	ld	a4,-24(s0)
	ld	a5,-56(s0)
	bltu	a4,a5,.L12
	nop
	nop
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	vec_fromFileStream_i32, .-vec_fromFileStream_i32
	.section	.rodata
	.align	3
.LC6:
	.string	"%f"
	.align	3
.LC7:
	.string	"Failed to read float32 type from filestream.\n"
	.text
	.align	1
	.globl	vec_fromFileStream_f32
	.type	vec_fromFileStream_f32, @function
vec_fromFileStream_f32:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	zero,-24(s0)
	j	.L14
.L16:
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	mv	a2,a5
	lla	a1,.LC6
	ld	a0,-40(s0)
	call	__isoc99_fscanf@plt
	mv	a5,a0
	mv	a4,a5
	li	a5,1
	beq	a4,a5,.L15
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,45
	li	a1,1
	lla	a0,.LC7
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L15:
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L14:
	ld	a4,-24(s0)
	ld	a5,-56(s0)
	bltu	a4,a5,.L16
	nop
	nop
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	vec_fromFileStream_f32, .-vec_fromFileStream_f32
	.section	.rodata
	.align	3
.LC8:
	.string	"%lf"
	.align	3
.LC9:
	.string	"Failed to read float64 type from filestream.\n"
	.text
	.align	1
	.globl	vec_fromFileStream_f64
	.type	vec_fromFileStream_f64, @function
vec_fromFileStream_f64:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	zero,-24(s0)
	j	.L18
.L20:
	ld	a5,-24(s0)
	slli	a5,a5,3
	ld	a4,-48(s0)
	add	a5,a4,a5
	mv	a2,a5
	lla	a1,.LC8
	ld	a0,-40(s0)
	call	__isoc99_fscanf@plt
	mv	a5,a0
	mv	a4,a5
	li	a5,1
	beq	a4,a5,.L19
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,45
	li	a1,1
	lla	a0,.LC9
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L19:
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L18:
	ld	a4,-24(s0)
	ld	a5,-56(s0)
	bltu	a4,a5,.L20
	nop
	nop
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	ra
	.size	vec_fromFileStream_f64, .-vec_fromFileStream_f64
	.align	1
	.globl	dotproduct_plain_i8
	.type	dotproduct_plain_i8, @function
dotproduct_plain_i8:
	addi	sp,sp,-64
	sd	s0,56(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	a3,-64(s0)
	sd	zero,-24(s0)
	j	.L22
.L23:
	ld	a4,-40(s0)
	ld	a5,-24(s0)
	add	a5,a4,a5
	lbu	a3,0(a5)
	ld	a4,-48(s0)
	ld	a5,-24(s0)
	add	a5,a4,a5
	lbu	a4,0(a5)
	ld	a2,-56(s0)
	ld	a5,-24(s0)
	add	a5,a2,a5
	mulw	a4,a3,a4
	andi	a4,a4,0xff
	sb	a4,0(a5)
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L22:
	ld	a4,-24(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L23
	nop
	nop
	ld	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	dotproduct_plain_i8, .-dotproduct_plain_i8
	.align	1
	.globl	dotproduct_plain_i16
	.type	dotproduct_plain_i16, @function
dotproduct_plain_i16:
	addi	sp,sp,-64
	sd	s0,56(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	a3,-64(s0)
	sd	zero,-24(s0)
	j	.L25
.L26:
	ld	a5,-24(s0)
	slli	a5,a5,1
	ld	a4,-40(s0)
	add	a5,a4,a5
	lh	a5,0(a5)
	slli	a4,a5,48
	srli	a4,a4,48
	ld	a5,-24(s0)
	slli	a5,a5,1
	ld	a3,-48(s0)
	add	a5,a3,a5
	lh	a5,0(a5)
	slli	a5,a5,48
	srli	a5,a5,48
	mulw	a5,a4,a5
	slli	a3,a5,48
	srli	a3,a3,48
	ld	a5,-24(s0)
	slli	a5,a5,1
	ld	a4,-56(s0)
	add	a5,a4,a5
	slliw	a4,a3,16
	sraiw	a4,a4,16
	sh	a4,0(a5)
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L25:
	ld	a4,-24(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L26
	nop
	nop
	ld	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	dotproduct_plain_i16, .-dotproduct_plain_i16
	.align	1
	.globl	dotproduct_plain_i32
	.type	dotproduct_plain_i32, @function
dotproduct_plain_i32:
	addi	sp,sp,-64
	sd	s0,56(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	a3,-64(s0)
	sd	zero,-24(s0)
	j	.L28
.L29:
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a3,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	lw	a4,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a2,-56(s0)
	add	a5,a2,a5
	mulw	a4,a3,a4
	sext.w	a4,a4
	sw	a4,0(a5)
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L28:
	ld	a4,-24(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L29
	nop
	nop
	ld	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	dotproduct_plain_i32, .-dotproduct_plain_i32
	.align	1
	.globl	dotproduct_plain_f32
	.type	dotproduct_plain_f32, @function
dotproduct_plain_f32:
	addi	sp,sp,-64
	sd	s0,56(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	a3,-64(s0)
	sd	zero,-24(s0)
	j	.L31
.L32:
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	flw	fa4,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	flw	fa5,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-56(s0)
	add	a5,a4,a5
	fmul.s	fa5,fa4,fa5
	fsw	fa5,0(a5)
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L31:
	ld	a4,-24(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L32
	nop
	nop
	ld	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	dotproduct_plain_f32, .-dotproduct_plain_f32
	.align	1
	.globl	dotproduct_plain_f64
	.type	dotproduct_plain_f64, @function
dotproduct_plain_f64:
	addi	sp,sp,-64
	sd	s0,56(sp)
	addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	sd	a3,-64(s0)
	sd	zero,-24(s0)
	j	.L34
.L35:
	ld	a5,-24(s0)
	slli	a5,a5,3
	ld	a4,-40(s0)
	add	a5,a4,a5
	fld	fa4,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,3
	ld	a4,-48(s0)
	add	a5,a4,a5
	fld	fa5,0(a5)
	ld	a5,-24(s0)
	slli	a5,a5,3
	ld	a4,-56(s0)
	add	a5,a4,a5
	fmul.d	fa5,fa4,fa5
	fsd	fa5,0(a5)
	ld	a5,-24(s0)
	addi	a5,a5,1
	sd	a5,-24(s0)
.L34:
	ld	a4,-24(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L35
	nop
	nop
	ld	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	dotproduct_plain_f64, .-dotproduct_plain_f64
	.section	.rodata
	.align	3
.LC10:
	.string	"Failed to allocate memory for vec1\n"
	.align	3
.LC11:
	.string	"Failed to allocate memory for vec2\n"
	.align	3
.LC12:
	.string	"Failed to allocate memory for result\n"
	.align	3
.LC14:
	.string	"Dot product time: %f seconds\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-96
	sd	ra,88(sp)
	sd	s0,80(sp)
	addi	s0,sp,96
	li	a0,0
	call	time@plt
	mv	a5,a0
	sext.w	a5,a5
	mv	a0,a5
	call	srand@plt
	li	a5,7999488
	addi	a5,a5,512
	sd	a5,-72(s0)
	ld	a0,-72(s0)
	call	malloc@plt
	mv	a5,a0
	sd	a5,-64(s0)
	ld	a5,-64(s0)
	bne	a5,zero,.L37
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,35
	li	a1,1
	lla	a0,.LC10
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L37:
	ld	a2,-72(s0)
	li	a1,0
	ld	a0,-64(s0)
	call	memset@plt
	sd	zero,-88(s0)
	j	.L38
.L39:
	call	rand@plt
	mv	a5,a0
	mv	a4,a5
	sraiw	a5,a4,31
	srliw	a5,a5,24
	addw	a4,a4,a5
	andi	a4,a4,255
	subw	a5,a4,a5
	sext.w	a3,a5
	ld	a4,-64(s0)
	ld	a5,-88(s0)
	add	a5,a4,a5
	andi	a4,a3,0xff
	sb	a4,0(a5)
	ld	a5,-88(s0)
	addi	a5,a5,1
	sd	a5,-88(s0)
.L38:
	ld	a4,-88(s0)
	ld	a5,-72(s0)
	bltu	a4,a5,.L39
	ld	a0,-72(s0)
	call	malloc@plt
	mv	a5,a0
	sd	a5,-56(s0)
	ld	a5,-56(s0)
	bne	a5,zero,.L40
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,35
	li	a1,1
	lla	a0,.LC11
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L40:
	ld	a2,-72(s0)
	li	a1,0
	ld	a0,-56(s0)
	call	memset@plt
	sd	zero,-80(s0)
	j	.L41
.L42:
	call	rand@plt
	mv	a5,a0
	mv	a4,a5
	sraiw	a5,a4,31
	srliw	a5,a5,24
	addw	a4,a4,a5
	andi	a4,a4,255
	subw	a5,a4,a5
	sext.w	a3,a5
	ld	a4,-56(s0)
	ld	a5,-80(s0)
	add	a5,a4,a5
	andi	a4,a3,0xff
	sb	a4,0(a5)
	ld	a5,-80(s0)
	addi	a5,a5,1
	sd	a5,-80(s0)
.L41:
	ld	a4,-80(s0)
	ld	a5,-72(s0)
	bltu	a4,a5,.L42
	ld	a0,-72(s0)
	call	malloc@plt
	mv	a5,a0
	sd	a5,-48(s0)
	ld	a5,-48(s0)
	bne	a5,zero,.L43
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,37
	li	a1,1
	lla	a0,.LC12
	call	fwrite@plt
	li	a0,1
	call	exit@plt
.L43:
	ld	a2,-72(s0)
	li	a1,0
	ld	a0,-48(s0)
	call	memset@plt
	call	clock@plt
	sd	a0,-40(s0)
	ld	a3,-72(s0)
	ld	a2,-48(s0)
	ld	a1,-56(s0)
	ld	a0,-64(s0)
	call	dotproduct_plain_i8
	call	clock@plt
	sd	a0,-32(s0)
	ld	a4,-32(s0)
	ld	a5,-40(s0)
	sub	a5,a4,a5
	fcvt.d.l	fa4,a5
	lla	a5,.LC13
	fld	fa5,0(a5)
	fdiv.d	fa5,fa4,fa5
	fsd	fa5,-24(s0)
	ld	a1,-24(s0)
	lla	a0,.LC14
	call	printf@plt
	li	a5,0
	mv	a0,a5
	ld	ra,88(sp)
	ld	s0,80(sp)
	addi	sp,sp,96
	jr	ra
	.size	main, .-main
	.section	.rodata
	.align	3
.LC13:
	.word	0
	.word	1093567616
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
