	.file	"extreme_test.c"
	.text
	.p2align 4
	.globl	extreme_optimization_test
	.type	extreme_optimization_test, @function
extreme_optimization_test:
.LFB0:
	.cfi_startproc
	endbr64
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	xorl	%esi, %esi
	movl	$8, %edx
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	movabsq	$578437695752307201, %rax
	movq	%rsp, %rdi
	movq	%rax, (%rsp)
	call	smemset@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L5
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L5:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE0:
	.size	extreme_optimization_test, .-extreme_optimization_test
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	xorl	%esi, %esi
	movl	$8, %edx
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	movabsq	$578437695752307201, %rax
	movq	%rsp, %rdi
	movq	%rax, (%rsp)
	call	smemset@PLT
	movq	8(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L9
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
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
