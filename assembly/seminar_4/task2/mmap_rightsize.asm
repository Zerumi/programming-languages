; mmap_rightsize.asm
%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2
%define SYS_WRITE 1
%define SYS_OPEN 2
%define SYS_FSTAT 5
%define SYS_MMAP 9
%define SYS_MUNMAP 11
%define SYS_CLOSE 3
%define FD_STDOUT 1
%define FSTAT_BUF 144


section .bss
fstat: resb FSTAT_BUF

section .data
fname: db "test.txt", 0

section .text
global _start

; use exit system call to shut down correctly
exit:
    mov  rax, 60
    xor  rdi, rdi
    syscall

; These functions are used to print a null terminated string
; rdi holds a string pointer
print_string:
    push rdi
    call string_length
    pop  rsi
    mov  rdx, rax 
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

string_length:
    xor  rax, rax
.loop:
    cmp  byte [rdi+rax], 0
    je   .end 
    inc  rax
    jmp .loop 
.end:
    ret

; This function is used to print a substring with given length
; rdi holds a string pointer
; rsi holds a substring length
print_substring:
    mov  rdx, rsi 
    mov  rsi, rdi
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

_start:
	push r13
	push r14

	mov rax, SYS_OPEN 	; open syscall
	mov rdi, fname		; put fname as first argument
	mov rsi, O_RDONLY	; read only modifier as second argument
	xor rdx, rdx 		; we are not creating file, useless argument
	syscall			; magic here
	
	mov r14, rax		; in rax - fd
	mov rdi, rax
	mov rax, SYS_FSTAT
	mov rsi, fstat
	syscall

	mov r8, r14		; in r14 - file descriptor. Use as fifth syscall argument
	mov rax, 9		; mmap syscall
	xor rdi, rdi		; let OS choose mapping destination - first argument
	mov rsi, [fstat + 48] 	; page size - second argument
	mov rdx, PROT_READ	; readonly memory region - third argument
	mov r10, MAP_PRIVATE	; private memory page - fourth argument
	xor r9, r9		; no file offset - sixth argument
	syscall			; magic here

	mov rdi, rax
	mov rsi, [fstat + 48]
	mov r13, rax
	call print_substring
	
	mov rax, SYS_MUNMAP
	mov rdi, r13
	mov rsi, [fstat + 48]
	syscall
	
	mov rax, SYS_CLOSE
	mov rdi, r14
	syscall

	pop r14
	pop r13	

	call exit
