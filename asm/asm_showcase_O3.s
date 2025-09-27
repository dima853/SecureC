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
	movq	$3355185, 16(%rsp)
	movaps	%xmm0, (%rsp)
	movq	%rbx, %rdx
	pxor	%xmm0, %xmm0
	movaps	%xmm0, 32(%rsp)
	movaps	%xmm0, 48(%rsp)
	movq	$0, 24(%rsp)
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
	.size	test_smemset_clear, .-test_smemset_clear
	.section	.rodata.str1.1
.LC11:
	.string	"Buffer filled: %.10s...\n"
.LC12:
	.string	"Buffer after: %.10s...\n"
	.text
	.p2align 4
	.globl	test_smemset_with_calc
	.type	test_smemset_with_calc, @function
test_smemset_with_calc:
.LFB24:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC11(%rip), %rsi
	movl	$2, %edi
	subq	$112, %rsp
	.cfi_def_cfa_offset 128
	movdqa	.LC4(%rip), %xmm0
	movq	%fs:40, %rax
	movq	%rax, 104(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbx
	movl	$1448432723, 96(%rsp)
	movaps	%xmm0, (%rsp)
	movdqa	.LC5(%rip), %xmm0
	movq	%rbx, %rdx
	movaps	%xmm0, 16(%rsp)
	movdqa	.LC6(%rip), %xmm0
	movaps	%xmm0, 32(%rsp)
	movdqa	.LC7(%rip), %xmm0
	movaps	%xmm0, 48(%rsp)
	movdqa	.LC8(%rip), %xmm0
	movaps	%xmm0, 64(%rsp)
	movdqa	.LC9(%rip), %xmm0
	movaps	%xmm0, 80(%rsp)
	call	__printf_chk@PLT
	movl	$100, %edx
	movl	$88, %esi
	movq	%rbx, %rdi
	call	smemset@PLT
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movl	$2, %edi
	leaq	.LC12(%rip), %rsi
	call	__printf_chk@PLT
	movq	104(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L9
	addq	$112, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	test_smemset_with_calc, .-test_smemset_with_calc
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC13:
	.string	"\360\237\224\215 ASM Showcase for smemset\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC13(%rip), %rdi
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
.LFE25:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.quad	8387235652274714957
	.quad	7237970109966541136
	.align 16
.LC4:
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.byte	71
	.byte	72
	.byte	73
	.byte	74
	.byte	75
	.byte	76
	.byte	77
	.byte	78
	.byte	79
	.byte	80
	.align 16
.LC5:
	.byte	81
	.byte	82
	.byte	83
	.byte	84
	.byte	85
	.byte	86
	.byte	87
	.byte	88
	.byte	89
	.byte	90
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.align 16
.LC6:
	.byte	71
	.byte	72
	.byte	73
	.byte	74
	.byte	75
	.byte	76
	.byte	77
	.byte	78
	.byte	79
	.byte	80
	.byte	81
	.byte	82
	.byte	83
	.byte	84
	.byte	85
	.byte	86
	.align 16
.LC7:
	.byte	87
	.byte	88
	.byte	89
	.byte	90
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.byte	71
	.byte	72
	.byte	73
	.byte	74
	.byte	75
	.byte	76
	.align 16
.LC8:
	.byte	77
	.byte	78
	.byte	79
	.byte	80
	.byte	81
	.byte	82
	.byte	83
	.byte	84
	.byte	85
	.byte	86
	.byte	87
	.byte	88
	.byte	89
	.byte	90
	.byte	65
	.byte	66
	.align 16
.LC9:
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.byte	71
	.byte	72
	.byte	73
	.byte	74
	.byte	75
	.byte	76
	.byte	77
	.byte	78
	.byte	79
	.byte	80
	.byte	81
	.byte	82
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
