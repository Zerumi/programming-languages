%include "lib.inc"

%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

section .data
fname: db "test.txt", 0

section .text
global _start

_start:
	mov rax, 2 		; open syscall
	mov rdi, fname		; put fname as first argument
	mov rsi, O_RDONLY	; read only modifier as second argument
	xor rdx, rdx 		; we are not creating file, useless argument
	syscall			; magic here

	mov r8, rax		; in rax - file descriptor. Use as fifth syscall argument
	mov rax, 9		; mmap syscall
	xor rdi, rdi		; let OS choose mapping destination - first argument
	mov rsi, 4096 		; page size - second argument
	mov rdx, PROT_READ	; readonly memory region - third argument
	mov r10, MAP_PRIVATE	; private memory page - fourth argument
	xor r9, r9		; no file offset - sixth argument
	syscall			; magic here

	mov rdi, rax
	call print_string

	call exit
