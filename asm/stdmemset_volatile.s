	.file	"volatile_test_stdmemset.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Volatile before: %s\n"
.LC1:
	.string	"Volatile after: [CLEARED]"
	.text
	.p2align 4
	.globl	test_volatile_memset
	.type	test_volatile_memset, @function
test_volatile_memset:
.LFB37:
	.cfi_startproc
	endbr64
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movl	$2, %edi
	movdqa	.LC2(%rip), %xmm0
	leaq	.LC0(%rip), %rsi
	movq	%fs:40, %rax
	movq	%rax, 72(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rdx
	movaps	%xmm0, (%rsp)
	pxor	%xmm0, %xmm0
	movq	$24948, 16(%rsp)
	movq	$0, 24(%rsp)
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	call	__printf_chk@PLT
	pxor	%xmm0, %xmm0
	leaq	.LC1(%rip), %rdi
	movaps	%xmm0, (%rsp)
	movaps	%xmm0, 16(%rsp)
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	call	puts@PLT
	movq	72(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L5
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L5:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE37:
	.size	test_volatile_memset, .-test_volatile_memset
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB38:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	call	test_volatile_memset
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE38:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.quad	7308332243887091542
	.quad	7008854899154969939
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
