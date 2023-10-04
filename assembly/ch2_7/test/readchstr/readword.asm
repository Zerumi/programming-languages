global _start

section .data
buffer: times 8 db 0

section .text

global read_char
global read_word
global string_length
global print_string

read_char:
    	xor rax, rax
    	xor rdi, rdi
    	dec rsp
   	mov rsi, rsp
   	mov rdx, 1
  	syscall
    	test rax, rax
    	jz .EOF
    	mov al, [rsp]
    	jmp .END
    	.EOF:
    	xor rax, rax
    	.END:
    	inc rsp
    	ret

read_word:
    ; in rdi - buffer
    ; in rsi - size
    ; in rax - buffer
	; A:
	; whitespace -> A
	; char -> B
	; char & buffer -> D
	; EOF -> D
	; OF -> E
	; B:
	; whitespace -> D
	; char -> B
	; OF -> E
	; D:
	; return buffer
	; E:
	; return 0
	xor rcx, rcx
	mov rcx, rsi
	dec rsi
.clear_buf:
	test rcx, rcx
	jz _A
	mov byte[rdi + rcx], 0
	dec rcx
	jmp .clear_buf
_A:
	push rdi
	push rsi
	push rcx
	call read_char
	pop rcx
	pop rsi
	pop rdi
	test al, al
	jz _D
	cmp al, 0x9
	je _A
	cmp al, 0xA
	je _A
	cmp al, 0x20
	je _A

	cmp rcx, rsi
	je _E
	
	mov byte[rdi + rcx], al
	inc rcx
	jmp _B
_B:
	push rdi
	push rsi
	push rcx
	call read_char
	pop rcx
	pop rsi
	pop rdi
	
	test al, al
	jz _D
	cmp al, 0x9
	je _D
	cmp al, 0xA
	je _D
	cmp al, 0x20
	je _D

	cmp rcx, rsi
	je _E

	mov byte[rdi + rcx], al
	inc rcx
	jmp _B
_D:
	; commented out code is for reversing string chars
	; but it's not needed in this program
	; but i wrote it....
	; don't ask why....
	; my life is trash
	; pls help me
	; if you are reading this, don't cry... please....
	; prepare for bad time, bro :)
	; ! reverse order
	; in rdi - buff addr
	; in rcx - amount of symbols
	;xor rdx, rdx
	;xor r12, r12
	;mov r13, rcx

	;mov rax, rcx
	;mov r9, 2
	;div r9
	;mov r10, rax
;.reverse:
	;test r10, r10
	;jz .exit

	;mov r11b, byte[rdi + rcx]
	; mov byte[rdi + rcx], byte[rdi + r12] is not working
	;mov r14b, byte[rdi + r12]
	;mov byte[rdi + rcx], r14b
	;mov byte[rdi + r12], r11b 
	;
	;inc r12
	;dec rcx
	;dec r10
	;jmp .reverse
;.exit:
	push rdi
	push rcx
	call string_length
	pop rcx
	pop rdi
	mov rdx, rax
	inc rcx
	xor al, al
	mov byte[rdi + rcx], al
	mov rax, rdi
	ret 
_E:
	xor rdx, rdx
	xor rax, rax
	ret


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

_start:
	; call read_char
	mov rdi, buffer
	mov rsi, 8
	call read_word
	mov rdi, rdx
	mov rax, 60
	syscall
