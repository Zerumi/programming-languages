global _start

section .data
message: db 'girls sometimes are beautiful', 10

section .text
_start:
	mov	rax, 1		; syscall number (write) should be stored in rax
	mov	rdi, 1		; argument #1 in rdi: file descriptor
	mov	rsi, message	; argument #2 in rsi: string start (start index for string register)
	mov	rdx, 30		; argument #3 in rdx: amount of symbols
	syscall			; magic here


	mov	rax, 60		; syscall number (exit) should be stored in rax
	xor	rdi, rdi	; clear register
	syscall 		; magic here. again. shit, this is an assembly language!
