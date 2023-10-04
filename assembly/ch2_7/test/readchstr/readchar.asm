global _start

section .text

global read_char

read_char:
    xor rax, rax
    xor rdi, rdi
    dec rsp
    mov rsi, rsp
    mov rdx, 1
    syscall
    test rax, rax
    jz .EOF
    mov al, [rsp]
    jmp .END
    .EOF:
    xor rax, rax
    .END:
    inc rsp
    ret

_start:
	call read_char
	mov rdi, rax
	mov rax, 60
	syscall
