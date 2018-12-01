GLOBAL DebugBreak
GLOBAL __hlt

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