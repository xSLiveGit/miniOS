%macro PUSH_A 0
    push rax 
    push rbx 
    push rcx 
    push rdx 
    push rsi 
    push rdi 
    push rbp 
    push r8 
    push r9
    push r10 
    push r11 
    push r12 
    push r13 
    push r14
    push r15 
    pushf
%endmacro

%macro POP_A 0
    popf
    pop r15
    pop r14 
    pop r13 
    pop r12 
    pop r11 
    pop r10 
    pop r9 
    pop r8 
    pop rbp 
    pop rdi 
    pop rsi 
    pop rdx 
    pop rcx 
    pop rbx 
    pop rax 
%endmacro

%define ICW_1 0x11				        ; 00010001 binary. Enables initialization mode and we are sending ICW 4

%define ICW_3_MASTER    0x4		        ; 0x04 => 0100, second bit (IR line 2)
%define ICW_3_SLAVE     0X2		        ; 010=> IR line 2

%define PIC_MASTER_CTRL 0x20			; Primary PIC control register
%define PIC_SLAVE_CTRL 0xA0				; Secondary PIC control register
 
%define PIC_MASTER_DATA 0x21			; Primary PIC data register
%define PIC_SLAVE_DATA 0xA1				; Secondary PIC data register
 
%define IRQ_0	0x20				; IRQs 0-7 mapped to use interrupts 0x20-0x27
%define IRQ_8	0x28				; IRQs 8-15 mapped to use interrupts 0x28-0x36

%define PIC_EOI		    0x20	

%define KYBRD_ENC_INPUT_BUF	    0x60
%define KYBRD_ENC_CMD_REG	    0x60
%define KYBRD_CTRL_STATS_REG    0x64
%define KYBRD_CTRL_CMD_REG      0x64


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


GLOBAL _IntAsmIsrBasic
extern IsrBasic
; void _IntAsmIsrBasic(void)
_IntAsmIsrBasic:
    PUSH_A
    cli
	call IsrBasic
    POP_A
    sti
	iretq 


GLOBAL _IntAsmIsrCritical
extern IsrCritical;
; void _IntAsmIsrCritical(void)
_IntAsmIsrCritical:
    PUSH_A
    cli
	call IsrCritical
    sti
    POP_A
    iretq


GLOBAL _IntAsmIsrKeyboard
extern IsrKeyboard
; void _IntAsmIsrKeyboard(void)
_IntAsmIsrKeyboard:
    PUSH_A
    cli
	call IsrKeyboard
    sti
    POP_A
	iretq


GLOBAL _IntAsmIsrTimer
extern IsrTimer;
; void _IntAsmIsrTimer(void)
_IntAsmIsrTimer:
    PUSH_A
    cli
	call IsrTimer
    sti
    POP_A
	iretq