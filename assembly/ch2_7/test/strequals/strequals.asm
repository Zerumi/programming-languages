global string_equals
global _start

section .data
str1: db 'abcdef gh', 0
str2: db 'abcdef gh', 0

section .text

string_equals:
	xor r9, r9
	xor rcx, rcx
_A:
	mov r9b, byte[rdi + rcx]
	cmp r9b, byte[rsi + rcx]
	jne _E
	test r9, r9
	jz _D
	inc rcx
	jmp _A
_D:
	mov rax, 1
	ret
_E:
	mov rax, 0
	ret

_start:
	mov rdi, str1
	mov rsi, str2
	call string_equals
	mov rdi, rax
	mov rax, 60
	syscall
