	.file	"extreme_test_stdmemset.c"
	.text
	.p2align 4
	.globl	extreme_optimization_test
	.type	extreme_optimization_test, @function
extreme_optimization_test:
.LFB14:
	.cfi_startproc
	endbr64
	ret
	.cfi_endproc
.LFE14:
	.size	extreme_optimization_test, .-extreme_optimization_test
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB15:
	.cfi_startproc
	endbr64
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE15:
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
