section .data

teststr: db 'abcdef', 0

section .text

global string_length

string_length:
	xor rax, rax
.loop:
	cmp byte[rdi + rax], 0
	je .exit
	inc rax
	jmp .loop
.exit:
	ret

global _start

_start:
	mov rdi, teststr
	call string_length

	mov rdi, rax
	mov rax, 60
	syscall

