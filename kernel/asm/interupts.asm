
%include "trapframe.inc"
%include "interupts.inc"
%include "utils.inc"

GLOBAL IntInitPic
GLOBAL IntAsmBasic
GLOBAL IntAsmLidt;
GLOBAL IntAsmTrapFrame;
extern AsmIntDumpTrapFrame;
extern TrapFrame64Dump;
extern gTrapFrame;

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
IntAsmTrapFrame:
 	cli
    
    ; xchg bx, bx
    ; call AsmIntDumpTrapFrame
    ; xchg bx, bx

    COMPLETE_TRAPGRAME_FIELDS gTrapFrame;

    ; TrapFrame64Dump(&gTrapFrame)
    mov rcx, gTrapFrame ; put PTRAP_FRAME in rcx
    call TrapFrame64Dump 

    mov al, PIC_EOI
	out PIC_MASTER_CTRL, al

    xchg bx, bx

    sti
    iretq

; void IntInitPic(void)
IntInitPic:

    push rbp 
    mov rbp, rsp 
    
    PUSH_A

	; Setup to initialize the primary PIC. Send ICW 1
    ; Bit 0 - Set to 1 so we can sent ICW 4
    ; Bit 1 - PIC cascading bit. x86 architectures have 2 PICs, so we need the primary PIC cascaded with the slave. Keep it 0
    ; Bit 2 - CALL address interval. Ignored by x86 and kept at 8, so keep it 0
    ; Bit 3 - Edge triggered/Level triggered mode bit. By default, we are in edge triggered, so leave it 0
    ; Bit 4 - Initialization bit. Set to 1
    ; Bits 5...7 - Unused on x86, set to 0.

    ; Send ICW1
	mov	al, ICW_1 ; Bit 1 & 4
	out	PIC_MASTER_CTRL, al
 
	; Remember that we have 2 PICs. Because we are cascading with this second PIC, send ICW 1 to second PIC command register
	out	PIC_SLAVE_CTRL, al	; slave PIC command register

    ; Send ICW2 - used to map the base address of the IVT of which the PIC are to use.
    mov	al, IRQ_0		; Primary PIC handled IRQ 0..7. IRQ 0 is now mapped to interrupt number 0x20
	out	PIC_MASTER_DATA, al

    mov al, IRQ_8       ; Secondary PIC handles IRQ's 8..15. IRQ 8 is now mapped to use interrupt 0x28
    out PIC_SLAVE_DATA, al

    ; ICW3
	mov	al, ICW_3_MASTER
	out	PIC_MASTER_DATA, al
 
	mov	al, ICW_3_SLAVE
	out	PIC_SLAVE_DATA, al	
    ; both PICs are connected to use IR line 2 to communicate with each other. We have also set a base interrupt number for both PICs to use.

    ; ICW4
    ; Initialization Control Word (ICW) 4
    ; Bit Number	Value	Description
    ; 0	uPM	If set (1), it is in 80x86 mode. Cleared if MCS-80/86 mode
    ; 1	AEOI	If set, on the last interrupt acknowledge pulse, controller automatically performs End of Interrupt (EOI) operation
    ; 2	M/S	Only use if BUF is set. If set (1), selects buffer master. Cleared if buffer slave.
    ; 3	BUF	If set, controller operates in buffered mode
    ; 4	SFNM	Special Fully Nested Mode. Used in systems with a large amount of cascaded controllers.
    ; 5-7	0	Reserved, must be 0
    
    mov	al, 1		; bit 0 enables 80x86 mode
	; send ICW 4 to both primary and secondary PICs
	out	PIC_MASTER_DATA, al
	out	PIC_SLAVE_DATA, al

    POP_A
    leave
    ; mov rsp, rbp
    ; pop rbp
    
    ret
