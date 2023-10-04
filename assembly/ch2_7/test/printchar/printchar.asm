global string_length
global print_string
global print_char
global _start

section .data

teststr: db 'girls sometimes are beautiful', 0
char_buffer: db 0

section .text

exit:
	mov rax, 60
	syscall

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

print_char:
	mov [char_buffer], rdi
	lea rdi, [char_buffer]
	call print_string
	ret

_start:
	mov rdi, 'gsfd'
	call print_char

	xor rdi, rdi
	call exit
