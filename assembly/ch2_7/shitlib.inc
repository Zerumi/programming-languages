section .text

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


exit:
	mov rax, 60
	syscall

string_length:
	; return value in rax firstly; and rdx secondly!
	; arguments taken from rdi, rsi, rdx, rcx, r8, r9 + from stack
	; str_pointer -> count
	
	xor rax, rax 		; zeroing rax

.loop:
	cmp [rdi + rax], 0	; is a nul-terminated char???
	je .exit		; yes -- ON EXIT!
	
	inc rax			; no? increasing rax and trying again
	jmp .loop

.exit:
	ret			; It's an exit!

print_string:
	; str_pointer -> stdout
	pop rdi
	call string_length 	; in rax - string length
	xor rcx, rcx 		; zeroing rcx
	mov rsi, rdi		; argument -> rsi
.loop:
	; in rsi - string from first char
	lea rdi, [rsi + rcx]
	call print_char
	inc rcx
	cmp rcx, rax
	je .exit
	jmp .loop
.exit:
	ret

print_char:			; / ! \ finish
	; char -> stdout
    	mov rax, 1 		; syscall (write) function
	mov rsi, rdi 		; move char to 2nd argument
	mov rdi, 1 		; write to stdout
	mov rdx, 1		; only one symbol
	syscall			; magic here
	ret			; this is the end

print_newline:			; / ! \ finish
    	mov rdi, 10		; print_char with fixed code
	call print_char		; and that's all
    	ret


print_uint:
    xor rax, rax
    ret


print_int:
    xor rax, rax
    ret

string_equals:
    xor rax, rax
    ret


read_char:
    xor rax, rax
    ret 

read_word:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
    ret
