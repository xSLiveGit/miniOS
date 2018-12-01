%include "utils.inc"

GLOBAL DebugBreak
GLOBAL __hlt
GLOBAL __sti
GLOBAL __cli
GLOBAL __lidt
GLOBAL __outb
GLOBAL __inb

DebugBreak:
    push rbp
    mov rbp, rsp

    xchg bx, bx

    pop rbp
    ret

; void __hlt(void)
__hlt:
	push rbp 
	mov rbp, rsp 

	; Interrupts and exceptions can "wake" a processor from halt state
	.halt:
	hlt
	jmp .halt

	pop rbp 
	ret 

; void __sti(void);
__sti:
    sti
    ret

; void __cli(void);
__cli:
    cli
    ret

; void __lidt(PVOID)
__lidt:
    lidt    [rcx]
    ret

; void __outb(uint16_t Port, uint8_t Data);
__outb:
    mov     rax,    rdx
    mov     rdx,    rcx
    out     dx,     al 
    ret

; uint8_t __inb(uint16_t Port);
__inb:
    mov     dx,     cx
    in      al,     dx
    ret