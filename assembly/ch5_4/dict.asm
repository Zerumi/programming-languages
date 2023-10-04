%include "lib.inc"

global find_word

section .data
error_msg1: db "No key found", 0

section .text

find_word:
	; in rsi + 8 - string pointer
	push r12
	push r13
	mov r12, rdi
	mov r13, rsi
	
	add rsi, 8
	call string_equals

	test rax, rax
	jnz .found
	; if rax = 0 (not the right key)
	; if [r12] = 0 -> error
	; -> mov rdi, r12
	; -> mov rsi, r13	
	; -> call find_word
	; -> jmp .exit
	mov rdi, r12
	mov rsi, [r13]
	test rsi, rsi
	jz .not_found
	call find_word
	jmp .exit
.not_found:
	mov rdi, error_msg1
	call print_error
	jmp .exit ; handle error / recursion exit
.found:
	; if rax = 1 (found the right key)
	; -> add r13, 8
	; -> mov rdi, r13
	; -> call string_length
	; -> add r13, rax
	; -> call print_string
	; -> ~~jmp .exit~~
	add r13, 8
	mov rdi, r13
	call string_length
	add r13, rax
	inc r13
	mov rdi, r13
	call print_string
.exit:
	pop r12
	pop r13
	ret
