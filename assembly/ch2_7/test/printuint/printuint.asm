global string_length
global print_string
global print_char
global print_newline
global print_uint
global _start

section .data
decimal_codes: db '0123456789'
teststr: db 'girls sometimes are beautiful', 0
char_buffer: db 0

section .text

exit:
	mov rax, 60
	syscall

string_length:
	xor rax, rax
.loop:
	cmp byte[rdi + rax], 0
	je .exit
	inc rax
	jmp .loop
.exit:
	ret

print_string:
	push rdi
	call string_length
	pop rdi
	inc rax
	mov rsi, rdi
	mov rdx, rax
	mov rdi, 1
	mov rax, 1
	syscall
	ret

print_char:
	mov [char_buffer], rdi
	lea rdi, [char_buffer]
	call print_string
	ret

print_newline:
	mov rdi, 10
	call print_char
	ret

print_uint:
	xor rcx, rcx
	mov rax, rdi
	mov r11, 10
.accumulate:
	xor rdx, rdx
	div r11
	lea rdi, [decimal_codes + rdx]
	mov r9, rdi
	xor rdi, rdi
	mov dil, byte[r9]
	push di
	inc rcx
	test rax, rax
	jne .accumulate
.print:
	test rcx, rcx
	je .exit
	dec rcx
	pop di
	push rcx
	call print_char
	pop rcx
	jmp .print
.exit:
	ret

_start:
	mov rdi, 'gsfd'
	call print_char
	
	call print_newline
	
	mov rdi, 400
	call print_uint
	call print_newline
	
	xor rdi, rdi
	call exit
