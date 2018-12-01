%define DEBUGBREAK xchg bx, bx
%define STRUCTURE_FIELD_OFFSET(Structure, Field) ((Field) - (Structure))

%include "trapframe.inc"
%include "utils.inc"

GLOBAL AsmIntDumpTrapFrame
GLOBAL gTrapFrame
GLOBAL __load_trap_frame;


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

; void __load_trap_frame(PTRAP_FRAME TrapFrame)
__load_trap_frame:
    push rbp
    mov rbp, rsp

    COMPLETE_TRAPGRAME_FIELDS rcx

    pop rbp
    ret
