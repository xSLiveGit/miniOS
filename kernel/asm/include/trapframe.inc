
%macro COMPLETE_TRAPGRAME_FIELDS 1
    mov [%1 + TRAP_FRAME.Rax], rax
    mov [%1 + TRAP_FRAME.Rbx], rbx
    mov [%1 + TRAP_FRAME.Rcx], rcx
    mov [%1 + TRAP_FRAME.Rdx], rdx
    mov [%1 + TRAP_FRAME.Rsi], rsi
    mov [%1 + TRAP_FRAME.Rdi], rdi
    mov [%1 + TRAP_FRAME.Rbp], rbp
    mov [%1 + TRAP_FRAME.R8],  r8
    mov [%1 + TRAP_FRAME.R9],  r9
    mov [%1 + TRAP_FRAME.R10], r10
    mov [%1 + TRAP_FRAME.R11], r11
    mov [%1 + TRAP_FRAME.R12], r12
    mov [%1 + TRAP_FRAME.R13], r13
    mov [%1 + TRAP_FRAME.R14], r14
    mov [%1 + TRAP_FRAME.R15], r15
    
    pushf
    pop QWORD [%1 + TRAP_FRAME.Flags]
%endmacro

STRUC TRAP_FRAME
    .Rax:   resq 1
    .Rbx:   resq 1
    .Rcx:   resq 1
    .Rdx:   resq 1 
    .Rsi:   resq 1 
    .Rdi:   resq 1 
    .Rbp:   resq 1 
    .Flags: resq 1 
    .R8:    resq 1
    .R9:    resq 1
    .R10:   resq 1
    .R11:   resq 1
    .R12:   resq 1
    .R13:   resq 1
    .R14:   resq 1
    .R15:   resq 1
ENDSTRUC
