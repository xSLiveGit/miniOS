
%define DEBUGBREAK xchg bx, bx

%define STRUCTURE_FIELD_OFFSET(Structure, Field) ((Field) - (Structure))

extern TrapFrame64Dump
extern LogCritical

GLOBAL AsmIntDumpTrapFrame

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

[Bits 64]
gTrapFrame: ISTRUC TRAP_FRAME
    AT TRAP_FRAME.Rax,      dq 0
    AT TRAP_FRAME.Rbx,      dq 0
    AT TRAP_FRAME.Rcx,      dq 0
    AT TRAP_FRAME.Rdx,      dq 0
    AT TRAP_FRAME.Rsi,      dq 0
    AT TRAP_FRAME.Rdi,      dq 0
    AT TRAP_FRAME.Rbp,      dq 0
    AT TRAP_FRAME.Flags,    dq 0
    AT TRAP_FRAME.R8,       dq 0
    AT TRAP_FRAME.R9,       dq 0
    AT TRAP_FRAME.R10,      dq 0
    AT TRAP_FRAME.R11,      dq 0
    AT TRAP_FRAME.R12,      dq 0
    AT TRAP_FRAME.R13,      dq 0
    AT TRAP_FRAME.R14,      dq 0
    AT TRAP_FRAME.R15,      dq 0
IEND


%macro CREATE_TRAP_FRAME 1
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

; void AsmIntDumpTrapFrame(void)
AsmIntDumpTrapFrame:
    push rbp
    mov rbp, rsp

    call LogCritical
    DEBUGBREAK

    mov rdx, 0xABCDEF
    mov rbx, 0xABCDEF
    mov rax, 0xABCDEF
    mov rcx, 0xABCDEF

    mov QWORD [gTrapFrame + TRAP_FRAME.Rax], rax
    mov QWORD [gTrapFrame + TRAP_FRAME.Rbx], rbx
    mov QWORD [gTrapFrame + TRAP_FRAME.Rcx], rcx
    mov QWORD [gTrapFrame + TRAP_FRAME.Rdx], rdx
    mov QWORD [gTrapFrame + TRAP_FRAME.Rsi], rsi
    mov QWORD [gTrapFrame + TRAP_FRAME.Rdi], rdi
    mov QWORD [gTrapFrame + TRAP_FRAME.Rbp], rbp
    mov QWORD [gTrapFrame + TRAP_FRAME.R8],  r8
    mov QWORD [gTrapFrame + TRAP_FRAME.R9],  r9
    mov QWORD [gTrapFrame + TRAP_FRAME.R10], r10
    mov QWORD [gTrapFrame + TRAP_FRAME.R11], r11
    mov QWORD [gTrapFrame + TRAP_FRAME.R12], r12
    mov QWORD [gTrapFrame + TRAP_FRAME.R13], r13
    mov QWORD [gTrapFrame + TRAP_FRAME.R14], r14
    mov QWORD [gTrapFrame + TRAP_FRAME.R15], r15
    
    ; mov QWORD [gTrapFrame + TRAP_FRAME.Flags], eflags

    pushf
    pop QWORD [gTrapFrame + TRAP_FRAME.Flags]
    ; mov [gTrapFrame + STRUCTURE_FIELD_OFFSET(TRAP_FRAME.Flags, Flags)], eflags

    DEBUGBREAK
    
   
    ; CREATE_TRAP_FRAME gTrapFrame

    mov QWORD [gTrapFrame + TRAP_FRAME.Rbp], 0xFEDCBA


    mov rcx, gTrapFrame ; put PTRAP_FRAME in rcx
    call TrapFrame64Dump ; void TrapFrameDump(PTRAP_FRAME)

    DEBUGBREAK

    pop rbp
    ret