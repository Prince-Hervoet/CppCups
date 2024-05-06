.globl GetContext, SetContext, SwapContext

GetContext:
    leaq 8(%rsp), %rax
    movq %rax, 0(%rcx)
    movq %rcx, %rax
    movq %rbp, 8(%rax)
    movq %rbx, 16(%rax)
    movq %rcx, 24(%rax)
    movq %rdx, 32(%rax)
    movq %rsi, 40(%rax)
    movq %rdi, 48(%rax)
    movq %r8, 56(%rax)
    movq %r9, 64(%rax)
    movq %r10, 72(%rax)
    movq %r11, 80(%rax)
    movq %r12, 88(%rax)
    movq %r13, 96(%rax)
    movq %r14, 104(%rax)
    movq %r15, 112(%rax)
    movq (%rsp), %rax
    movq %rax, 120(%rcx)
    movq $0, %rax
    retq


SetContext:
    movq %rcx, %rax
    movq 0(%rax), %rsp
    movq 8(%rax), %rbp
    movq 16(%rax), %rbx
    movq 24(%rax), %rcx
    movq 32(%rax), %rdx
    movq 40(%rax), %rsi
    movq 48(%rax), %rdi
    movq 56(%rax), %r8
    movq 64(%rax), %r9
    movq 72(%rax), %r10
    movq 80(%rax), %r11
    movq 88(%rax), %r12
    movq 96(%rax), %r13
    movq 104(%rax), %r14
    movq 112(%rax), %r15
    push 120(%rax)
    movq $0, %rax
    retq

SwapContext:
    leaq 8(%rsp), %rax
    movq %rax, 0(%rcx)
    movq %rcx, %rax
    movq %rbp, 8(%rax)
    movq %rbx, 16(%rax)
    movq %rcx, 24(%rax)
    movq %rdx, 32(%rax)
    movq %rsi, 40(%rax)
    movq %rdi, 48(%rax)
    movq %r8, 56(%rax)
    movq %r9, 64(%rax)
    movq %r10, 72(%rax)
    movq %r11, 80(%rax)
    movq %r12, 88(%rax)
    movq %r13, 96(%rax)
    movq %r14, 104(%rax)
    movq %r15, 112(%rax)
    movq (%rsp), %rax
    movq %rax, 120(%rcx)
    movq $0, %rax

    movq %rdx, %rax
    movq 0(%rax), %rsp
    movq 8(%rax), %rbp
    movq 16(%rax), %rbx
    movq 24(%rax), %rcx
    movq 32(%rax), %rdx
    movq 40(%rax), %rsi
    movq 48(%rax), %rdi
    movq 56(%rax), %r8
    movq 64(%rax), %r9
    movq 72(%rax), %r10
    movq 80(%rax), %r11
    movq 88(%rax), %r12
    movq 96(%rax), %r13
    movq 104(%rax), %r14
    movq 112(%rax), %r15
    push 120(%rax)
    movq $0, %rax
    retq
    

