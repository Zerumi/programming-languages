global string_length
global print_string
global _start

section .data

teststr: db 'girls sometimes are beautiful', 0

section .text

string_length:
	xor rax, rax
.loop:
	cmp byte[rdi + rax], 0
	je .exit
	inc rax
	jmp .loop
.exit:
	ret

print_string:
	push rdi
	call string_length
	pop rdi
	inc rax
	mov rsi, rdi
	mov rdx, rax
	mov rdi, 1
	mov rax, 1
	syscall
	ret

_start:
	mov rdi, teststr
	call print_string
	
	xor rdi, rdi
	mov rax, 60
	syscall
