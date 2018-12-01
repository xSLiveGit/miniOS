
%include "trapframe.inc"
%include "interupts.inc"
%include "utils.inc"
%include "keyboard.inc"

GLOBAL IntAsmBasic;
GLOBAL IntAsmLidt;
; GLOBAL IntAsmTrapFrame;

extern AsmIntDumpTrapFrame;
extern TrapFrame64Dump;
extern gTrapFrame;
extern IsrKeyboardKeyHandler;

; void IntAsmLidt(PIDT Idt)
IntAsmLidt:
    push rbp
	mov rbp, rsp

    cli
	lidt [rcx]
    sti
    
	pop rbp
	ret


; void IntAsmBasic(void)
IntAsmBasic:
 	cli
	PUSH_A

	mov al, PIC_EOI
	out PIC_MASTER_CTRL, al

	POP_A
	sti 
	iretq 

; void IntAsmTrapFrame(void)
; IntAsmTrapFrame:
;  	cli
    
;     ; xchg bx, bx
;     ; call AsmIntDumpTrapFrame
;     ; xchg bx, bx

;     COMPLETE_TRAPGRAME_FIELDS gTrapFrame;

;     ; TrapFrame64Dump(&gTrapFrame)
;     mov rcx, gTrapFrame ; put PTRAP_FRAME in rcx
;     call TrapFrame64Dump 

;     mov al, PIC_EOI
; 	out PIC_MASTER_CTRL, al

;     xchg bx, bx

;     sti
;     iretq


