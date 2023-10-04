section .text

global exit
exit:
	mov rax, 60
	syscall

global _start
_start:
	xor rdi, rdi
	call exit
