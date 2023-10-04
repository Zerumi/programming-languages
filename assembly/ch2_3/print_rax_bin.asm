section .data
codes:
	db	'0123456789ABCDEF'

section .text
global _start
_start:
	; number 1122... in hexdecimal format
	mov rax, 0x1122334455667788
	
	mov rdi, 1
	mov rdx, 1
	mov rcx, 64
	
.loop:
	push rax
	sub rcx, 1
	
	sar rax, cl ; signed shift right on cl bits
	and rax, 0x1 ; masking by 0000.0000.0000.0000.0000.0000.0000.0000.0000.0001
	
	lea rsi, [codes + rax] ; find symbol to print
	mov rax, 1 ; write syscall

	push rcx
	syscall
	pop rcx

	pop rax

	test rcx, rcx
	jnz .loop
	
	mov rax, 60
	xor rdi, rdi
	syscall
