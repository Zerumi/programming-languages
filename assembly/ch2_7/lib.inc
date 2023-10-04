global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global read_char
global read_word
global parse_uint
global parse_int
global string_equals
global string_copy

section .data						; useless section
decimal_codes: db '0123456789'
teststr: db 'girls sometimes are beautiful', 0

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
	pop rsi
	mov rdx, rax
	mov rdi, 1
	mov rax, 1
	syscall
	ret

print_newline:
	mov rdi, '\n'

print_char:
	push rdi
	lea rdi, [rsp]
	call print_string
	pop rdi
	ret

print_int:
	cmp rdi, 0
	jnl print_uint
	neg rdi
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi

print_uint:
	mov rax, rdi
	mov rdi, rsp
	sub rsp, 24
	mov byte[rdi - 1], 0
	dec rdi
	mov r11, 10
.accumulate:
	xor rdx, rdx
	div r11		; in rax - dividable num, in rdx - %
	add rdx, '0'
	dec rdi
	mov [rdi], dl
	test rax, rax
	jne .accumulate
	call print_string
	add rsp, 24
	ret

string_equals:
	xor r9, r9
	xor rcx, rcx
_seA:
	mov r9b, byte[rdi + rcx]
	cmp r9b, byte[rsi + rcx]
	jne _seE
	test r9, r9
	jz _seD
	inc rcx
	jmp _seA
_seD:
	mov rax, 1
	ret
_seE:
	mov rax, 0
	ret

read_char:
    	xor rax, rax
    	xor rdi, rdi
    	sub rsp, 16
   	mov rsi, rsp
   	mov rdx, 1
  	syscall
    	test rax, rax
    	jle .ERR
    	mov al, [rsp]
    	jmp .END
    	.ERR:
    	xor rax, rax
    	.END:
    	add rsp, 16
    	ret

read_word:
    ; in rdi - buffer -> r12
    ; in rsi - size   -> r13
    ; in rax - buffer -> r14
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

	push r12
	push r13
	push r15
	mov r12, rdi
	mov r13, rsi
	xor r15, r15

	mov r15, r13
	dec r13
	mov byte[r12], 0
.clear_buf:
	dec r15
	mov byte[r12 + r15], 0
	test r15, r15
	jnz .clear_buf
_A:
	call read_char
	test al, al
	jz _D
	cmp al, 0x9	; tabulation
	je _A
	cmp al, 0xA	; newline
	je _A
	cmp al, 0x20	; whitespace
	je _A

	cmp r15, r13
	je _E
	
	mov byte[r12 + r15], al
	inc r15
	jmp _B
_B:
	call read_char
	
	test al, al
	jz _D
	cmp al, 0x9	; tabulation
	je _D
	cmp al, 0xA	; newline
	je _D
	cmp al, 0x20	; whitespace
	je _D

	cmp r15, r13
	je _E

	mov byte[r12 + r15], al
	inc r15
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
	mov rdi, r12
	call string_length
	mov rdx, rax
	inc r15
	xor al, al
	mov byte[r12 + r15], al
	mov rax, r12
	pop r15
	pop r13
	pop r12
	ret 
_E:
	xor rdx, rdx
	xor rax, rax
	pop r15
	pop r13
	pop r12
	ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
	; in rdi - string
	xor rax, rax
	xor rcx, rcx
    	xor rdx, rdx
   	xor r9, r9
_puA:
    	mov al, byte[rdi + rcx]
	test al, al
	jz _puD
    	cmp al, '+'
    	je _puB
    	cmp al, '0'
    	jb _puD
    	cmp al, '9'
    	ja _puD
   	 
   	sub rax, 48
   	push ax
   	inc rdx
	
    	jmp _puC

_puB:
    	mov al, byte[rdi + rcx]
    	cmp al, '0'
    	jb _puD
    	cmp al, '9'
    	ja _puD

    	sub rax, 48
    	push ax
    	inc rdx

    	jmp _puC
    
_puC:
    	mov al, byte[rdi + rdx]
    	test al, al
    	jz _puD
    	cmp al, 10
    	jz _puD
    	cmp al, '0'
    	jb _puD
    	cmp al, '9'
    	ja _puD
    
    	sub rax, 48
    	push ax
    	inc rdx

    	jmp _puC
    
_puD:
    	mov r11, rdx
    	mov r10, 10
	test rdx, rdx
	jz .NaN
.loop:
	xor rax, rax
    	dec rdx
    	mov ax, word[rsp + rdx*2]
    	push rdx
    	mov rcx, rdx
.mul:
    	test rcx, rcx
    	jz .exit
    	dec rcx
    	mul r10
    	jmp .mul
.exit:
    	add r9, rax
    	pop rdx
    	test rdx, rdx
    	jne .loop
    	
    	add rsp, r11
	add rsp, r11
   	mov rdx, r11
    	mov rax, r9
	ret
.NaN:
	xor rax, rax
	xor rdx, rdx
	ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
	; in rdi - string
	xor rcx, rcx
	xor rax, rax
	xor rdx, rdx
_piA:
	mov al, byte[rdi]
	cmp al, '-'
	je _piMinus
_piPlus:
	jmp parse_uint
_piMinus:
	inc rdi
	call parse_uint
	neg rax
	inc rdx
	ret

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

