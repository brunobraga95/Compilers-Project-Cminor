.text
.global main
main:
	pushq %rbp
	movq  %rsp, %rbp
	subq $16,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	movq $2,%rbx
	movq %rbx,-8(%rbp)
	movq $2,%rbx
	movq %rbx,-16(%rbp)
	movq $1,%rbx
	movq $2,%r10
	movq %rbx,%rax
	cdq
	idivq %r10
	movq %rdx,%r10
	movq $3,%rbx
	movq %r10,%rax
	cdq
	idivq %rbx
	movq %rax,%rbx
	movq $4,%r10
	movq %rbx,%rax
	cdq
	idivq %r10
	movq %rdx,%r10
	movq $5,%rbx
	movq %r10,%rax
	imulq %rbx
	movq %rax,%rbx
	movq $6,%r10
	movq %rbx,%rax
	cdq
	idivq %r10
	movq %rdx,%r10
	movq $7,%rbx
	movq %r10,%rax
	cdq
	idivq %rbx
	movq %rax,%rbx
	movq $8,%r10
	movq %rbx,%rax
	cdq
	idivq %r10
	movq %rdx,%r10
	movq $9,%rbx
	addq %r10,%rbx
	movq $10,%r10
	movq $111,%r11
	movq %r10,%rax
	cdq
	idivq %r11
	movq %rdx,%r11
	movq $12,%r10
	movq %r11,%rax
	cdq
	idivq %r10
	movq %rax,%r10
	movq $13,%r11
	movq %r10,%rax
	cdq
	idivq %r11
	movq %rdx,%r11
	movq $14,%r10
	movq %r11,%rax
	imulq %r10
	movq %rax,%r10
	movq $15,%r11
	movq %r10,%rax
	cdq
	idivq %r11
	movq %rdx,%r11
	movq $16,%r10
	movq %r11,%rax
	cdq
	idivq %r10
	movq %rax,%r10
	movq $17,%r11
	movq %r10,%rax
	cdq
	idivq %r11
	movq %rdx,%r11
	addq %rbx,%r11
	pushq %r10
	pushq %r11
	movq %r11,%rdi
	call print_integer
	popq %r11
	popq %r10
FUNCAO0:

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
