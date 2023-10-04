section .data
teststr: db '-5', 0

section .text

; rdi - string pointer
; rsi - num chars
print_string:
    mov  rdx, rsi
    mov  rsi, rdi
    mov  rax, 1
    mov  rdi, 1
    syscall
    ret

global read_uint
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
    	jb _puE
    	cmp al, '9'
    	ja _puE
   	 
   	sub rax, 48
   	push ax
   	inc rdx
	
    	jmp _puC

_puB:
    	mov al, byte[rdi + rcx]
    	cmp al, '0'
    	jb _puE
    	cmp al, '9'
    	ja _puE

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
    	jb _puE
    	cmp al, '9'
    	ja _puE
    
    	sub rax, 48
    	push ax
    	inc rdx

    	jmp _puC
    
_puD:
    	mov r11, rdx
    	mov r10, 10
.loop:
    	dec rdx
    	mov ax, word[rsp + rdx*2]
    	push rdx
    	mov rcx, rdx
.mul:
    	test rcx, rcx
    	je .exit
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
_puE:
	add rsp, rdx
	add rsp, rdx
	mov rdx, 0
    	mov rax, 0
	ret

parse_int:
	; in rdi - string
_piA:
	mov al, byte[rdi]
	cmp al, '-'
	je _piMinus
_piPlus:
	call parse_uint
	ret
_piMinus:
	inc rdi
	call parse_uint
	neg rax
	ret

global _start
_start:
	mov rdi, teststr
    	call parse_int
	mov rdi, rax
	mov rax, 60
	syscall
