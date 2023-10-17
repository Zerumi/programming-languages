%include "lib.inc"
%include "dict.inc"
%include "words.inc"
%include "error_codes.inc"

%define POINTER_SIZE 8
%define BUFFER_SIZE 256

global _start

section .bss
buffer: resb BUFFER_SIZE

section .rodata
error_msg0: db "Unknown error occured", 10, 0
error_msg1: db "No key found", 10, 0
error_msg2: db "Buffer overflow / key should be less than 255 symbols", 10, 0

section .text
_start:
    	mov rdi, buffer
    	mov rsi, BUFFER_SIZE
	call read_line
	test rax, rax
	jnz .next
.error2:
	mov rdi, error_msg2
	call print_error
	call exit
.error0:
	mov rdi, error_msg0
	call print_error
	call exit
.error1:
	mov rdi, error_msg1
	call print_error
	call exit
.next:
	mov rdi, buffer
   	mov rsi, first_word
    	call find_word
	test eax, eax
	jnz .noerror
	test rdx, rdx
	jz .error0
	cmp rdx, NOT_FOUND
	je .error1
.noerror:
	; in rax - element. Output a value
	mov r9, rax
	add r9, POINTER_SIZE
	mov rdi, r9
	push r9
	call string_length
	pop r9
	add r9, rax
	inc r9
	mov rdi, r9
	call print_string
   	call exit

