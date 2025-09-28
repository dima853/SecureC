	.file	"volatile_test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Volatile before: %s\n"
.LC1:
	.string	"Volatile after: [CLEARED]"
	.text
	.p2align 4
	.globl	test_volatile_smemset
	.type	test_volatile_smemset, @function
test_volatile_smemset:
.LFB23:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC0(%rip), %rsi
	movl	$2, %edi
	subq	$80, %rsp
	.cfi_def_cfa_offset 96
	movdqa	.LC2(%rip), %xmm0
	movq	%fs:40, %rax
	movq	%rax, 72(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbx
	movaps	%xmm0, (%rsp)
	movq	%rbx, %rdx
	pxor	%xmm0, %xmm0
	movq	$24948, 16(%rsp)
	movq	$0, 24(%rsp)
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	call	__printf_chk@PLT
	xorl	%esi, %esi
	movq	%rbx, %rdi
	movl	$64, %edx
	call	smemset@PLT
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	movq	72(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L5
	addq	$80, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L5:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE23:
	.size	test_volatile_smemset, .-test_volatile_smemset
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	call	test_volatile_smemset
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
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
