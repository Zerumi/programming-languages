section .data

newline_char: db 10
codes: db '0123456789ABCDEF'

section .text
global _start

print_newline: 
	mov rax, 1		; syscall (write) function
	mov rdi, 1		; file descriptor
	mov rsi, newline_char	; string pointer
	mov rdx, 1		; amount of bytes
	syscall			; magic here
	ret			; ????

print_hex:
	mov rax, rdi
	
	mov rdi, 1
	mov rdx, 1
	mov rcx, 64
iterate:
	push rax		; save original value
	sub rcx, 4		; we are taking pack of 4 bits
	sar rax, cl		; signed shift right by cl bits (62, 58, 54, etc...)
	
	and rax, 0xf		; masking by hexadecimal number
	lea rsi, [codes + rax]	; find value char code
	
	mov rax, 1
	
	push rcx		; syscall will break rcx register!
	syscall			; magic here
	pop rcx			; restore rcx value

	pop rax 		; restore rax value
	
	test rcx, rcx 		; rcx = 0?
	jnz iterate 		; no -- repeat action

	ret			; ????

_start:
	mov rdi, 0x1122334455667788
	call print_hex
	call print_newline

	mov rax, 60		; syscall (exit) function
	xor rdi, rdi		; rdi = 0
	syscall			; magic here

