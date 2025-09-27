	.file	"asm_showcase.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Before: %s\n"
.LC1:
	.string	"After: [CLEARED]"
	.text
	.p2align 4
	.globl	test_smemset_clear
	.type	test_smemset_clear, @function
test_smemset_clear:
.LFB39:
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
	movq	$3355185, 16(%rsp)
	movaps	%xmm0, (%rsp)
	pxor	%xmm0, %xmm0
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	movq	$0, 24(%rsp)
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
.LFE39:
	.size	test_smemset_clear, .-test_smemset_clear
	.section	.rodata.str1.1
.LC5:
	.string	"Buffer filled: %.10s...\n"
.LC7:
	.string	"Buffer after: %.10s...\n"
	.text
	.p2align 4
	.globl	test_smemset_with_calc
	.type	test_smemset_with_calc, @function
test_smemset_with_calc:
.LFB40:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%edx, %edx
	subq	$112, %rsp
	.cfi_def_cfa_offset 128
	movq	%fs:40, %rax
	movq	%rax, 104(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbx
	.p2align 4,,10
	.p2align 3
.L7:
	movl	%edx, %eax
	movl	%edx, %ecx
	imulq	$1321528399, %rax, %rax
	shrq	$35, %rax
	imull	$26, %eax, %eax
	subl	%eax, %ecx
	leal	65(%rcx), %eax
	movb	%al, (%rbx,%rdx)
	addq	$1, %rdx
	cmpq	$100, %rdx
	jne	.L7
	movq	%rbx, %rdx
	leaq	.LC5(%rip), %rsi
	movl	$2, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movl	$2, %edi
	movdqa	.LC6(%rip), %xmm0
	leaq	.LC7(%rip), %rsi
	movl	$1482184792, 96(%rsp)
	movaps	%xmm0, (%rsp)
	movaps	%xmm0, 16(%rsp)
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	movaps	%xmm0, 64(%rsp)
	movaps	%xmm0, 80(%rsp)
	call	__printf_chk@PLT
	movq	104(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L11
	addq	$112, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L11:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE40:
	.size	test_smemset_with_calc, .-test_smemset_with_calc
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC8:
	.string	"\360\237\224\215 ASM Showcase for smemset\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB41:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	xorl	%eax, %eax
	call	test_smemset_clear
	xorl	%eax, %eax
	call	test_smemset_with_calc
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.quad	8387235652274714957
	.quad	7237970109966541136
	.align 16
.LC6:
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
	.byte	88
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
