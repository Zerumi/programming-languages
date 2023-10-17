%include "error_codes.inc"
%include "lib.inc"

%define POINTER_SIZE 8

global find_word

section .data
error_msg1: db "No key found", 10, 0

section .text

find_word:
	; in rdi / r12 - key
	; in rsi / r13 - last element
	; in rsi + 8 - string pointer
	; in rax - element with key (0 - see rdx, error may occur)
	; in rdx - error code (0 - normal result)
	push r12
	push r13

	test rdi, rdi
	jz .exit
	test rsi, rsi
	jz .exit

	mov r12, rdi
.loop:
	mov r13, rsi
	; in [r13] - next element	
	add rsi, POINTER_SIZE
	call string_equals

	test rax, rax
	jnz .found
	; if rax == 0 (not the right key)
	; if [r13] == 0 -> not found
	; -> mov rdi, r12, may break after call string_equals
	; -> mov rsi, [r13], get next element
	; -> jmp .loop if we need to continue
	mov rdi, r12
	mov rsi, [r13]
	test rsi, rsi
	jnz .loop
.not_found:
	xor eax, eax
	mov edx, NOT_FOUND
	jmp .exit
.found:
	; if rax = 1 (found the right key)
	xor rdx, rdx
	mov rax, r13
.exit:
	pop r12
	pop r13
	ret
