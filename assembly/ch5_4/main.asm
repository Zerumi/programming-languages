%include "lib.inc"
%include "dict.inc"
%include "words.inc"

global _start

section .bss
buffer: resb 255

section .data
error_msg2: db "Buffer overflow / key should be less than 255 symbols", 0

section .text
_start:
    	mov rdi, buffer
    	mov rsi, 255
	call read_line
	test rax, rax
	jnz .next
	mov rdi, error_msg2
	call print_error
	call exit
.next:
	mov rdi, buffer
   	mov rsi, first_word
    	call find_word
   	call exit

