global string_copy
global _start

section .data
teststr: db 'abcdef gh', 0
buffer: times 11 db 'AA'

section .text

string_copy:
	; in rdi - string addr
	; in rsi - buffer
	; in rdx - buffersize
	mov rcx, rdx
	dec rdx
	test rcx, rcx
	jz _scE
	mov byte[rsi], 0
.clear_buf:
	mov byte[rsi + rcx], 0
	dec rcx
	jnz .clear_buf
_scA:
	cmp rcx, rdx
	je _scB
	cmp byte[rdi + rcx], 0
	je _scD
	mov r9b, byte[rdi + rcx]
	mov byte[rsi + rcx], r9b
	inc rcx
	jmp _scA
_scB:
	cmp byte[rdi + rcx], 0
	je _scD
	jmp _scE
_scD:
	mov byte[rsi + rcx], 0
	mov rax, rsi
	mov rdi, 1
	ret
_scE:
	mov rax, 0
	mov rdi, 0
	ret

_start:
	mov rdi, teststr
	mov rsi, buffer
	mov rdx, 11
	call string_copy
	mov rax, 60
	syscall
	
