mov %rax, %rdi
add $37, %al
add $16, %ax
sub $0xff, %al
sub $0xff, %didd
nop
ret

mov %esp, %eax
add $0x37, %al
mov %eax, %edi