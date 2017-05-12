pop %rax

mov %eax, %edx
mov %edx, %ecx
mov %ecx, %esi
mov %rsp, %rax
mov %rax, %rdi
lea (%rdi, %rsi), %rax
mov %rax, %rdi
