section .text
global exit
global string_length
global print_string

exit:
    mov  rax, 60
    xor  rdi, rdi          
    syscall

string_length:
    mov  rax, rdi
  .counter:
    cmp  byte [rdi], 0
    je   .end
    inc  rdi
    jmp  .counter
  .end:
    sub  rdi, rax
    mov  rax, rdi
    ret

print_string:
    push rdi
    call string_length
    pop rdi
    mov  rdx, rax
    mov  rsi, rdi
    mov  rax, 1
    mov  rdi, 1
    syscall
    ret
