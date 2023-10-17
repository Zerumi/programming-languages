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
%define FSTAT_OFF 48

global print_string
global print_file

section .text

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

print_file:
	push r13
	push r14
	push r15

	mov rax, SYS_OPEN 	; open syscall
	mov rdi, rdi		; put fname as first argument
	mov rsi, O_RDONLY	; read only modifier as second argument
	xor rdx, rdx 		; we are not creating file, useless argument
	syscall			; magic here
	
	mov r15, rsp
	add rsp, FSTAT_BUF

	mov r14, rax		; in rax - fd
	mov rdi, rax
	mov rax, SYS_FSTAT
	mov rsi, r15
	syscall

	mov r8, r14		; in r14 - file descriptor. Use as fifth syscall argument
	mov rax, SYS_MMAP		; mmap syscall
	xor rdi, rdi		; let OS choose mapping destination - first argument
	mov rsi, [r15 + FSTAT_OFF] 	; page size - second argument
	mov rdx, PROT_READ	; readonly memory region - third argument
	mov r10, MAP_PRIVATE	; private memory page - fourth argument
	xor r9, r9		; no file offset - sixth argument
	syscall			; magic here

	mov rdi, rax
	mov rsi, [r15 + FSTAT_OFF]
	mov r13, rax
	call print_substring
	
	mov rax, SYS_MUNMAP
	mov rdi, r13
	mov rsi, [r15 + FSTAT_OFF]
	syscall
	
	mov rax, SYS_CLOSE
	mov rdi, r14
	syscall

	pop r15
	pop r14
	pop r13	

	call exit
