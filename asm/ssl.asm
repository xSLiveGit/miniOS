

%define ORIGIN_MBR 7C00h
%define ORIGIN_SSL 7E00h
%define ORIGIN_KERNEL 8000h
%define break xchg bx, bx

[bits 16]
[org ORIGIN_SSL]

SSLEntryPoint:
    ;/************************************************************
    ; *                     print a message
    ; ************************************************************/
    break   
    mov si, msg
    call StartPrint
    break 

    ;/***********************************************
    ;*                  Install GDT
    ;************************************************/
    call InstallGDT

    mov si, msg_GdtSuccess
    call    StartPrint 

    ;-------------------------------;
	;   Go into pmode		;
	;-------------------------------;
    break
	cli				    ; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax
    break
 
	jmp	08h:PMode32		; far jump to fix CS. Remember that the code selector is 0x8!

msg	db	"Installing GDT", '\n', 13, 0
msg_GdtSuccess db  "GDT was successfully installed", 0
msg_32Bits db "x86 mode is on", 0

;***************************************
;	Prints a string
;	DS=>SI: 0 terminated string
;***************************************

StartPrint:
    break

Print:
    lodsb
    or	        al, al				; al=current character
    jz	        PrintDone			; null terminator found
    mov	        ah,	0eh 			; get next character
    int	        10h
    jmp	        Print
    break

PrintDone:
    ret

;*******************************************
; InstallGDT()
;	- Install GDT
;*******************************************
 
InstallGDT:
 
	cli			    	        ; clear interrupts
	pusha				        ; save registers
    lgdt 	[Gdt32Descriptor]   ; load GDT into GDTR
	sti				            ; enable interrupts
	popa				        ; restore registers
	ret				            ; All done!
 

;*******************************************
; Global Descriptor Table (GDT)
;*******************************************
 
Gdt32Descriptor:
    .size       dw  Gdt32.end - Gdt32 - 1   ; GDT32 size
    .base       dd  Gdt32                   ; The linear address of the table itself

STRUC GlobalDescriptorTableEntry
    .segment_limit:  resw  1    ;*ignored in long mode
    .base_low:       resw  1    ;*ignored in long mode
    .base_middle:    resb  1    ;*ignored in long mode
    .access:         resb  1
	;=======================================================================================
	; ACCESS
	;  7 6 5 4 3 2 1 0 
	; |0|0|0|0|0|0|0|0|
	; 
	; Bit 0   : Access bit (Used with Virtual Memory)                                   *ignored in long mode
	; Bit 1   : 0 -> Read only (Data segments) / Execute only (Code segments)           *ignored in long mode
	;           1 -> Read and write (Data segments) / Execute and write (Code segments)
	; Bit 2   : Conforming (C) Bit Setting this bit to 1 identifies the code segment as conforming. 
    ;           When control is transferred to a higher-privilege conforming code-segment (C=1) from a lower-privilege code segment 
	; Bit 3   : Executable segment (0 for Data segments, 1 for Code segments)           *ignored in long mode
	; Bit 4   : Descriptor bit (0 for System descriptor, 1 for Code or Data descriptor) *ignored in long mode
	; Bit 5-6 : Descriptor Privilege level (Ring 0 - 3)
	; Bit 7   : Segment is in memory (Used with Virtual Memory)
	;=======================================================================================	 
    .granularity:    resb  1
	;=======================================================================================
	; GRANULARITY
	;  7 6 5 4 3 2 1 0 
	; |0|0|0|0|0|0|0|0|
	; 
	; Bit 0-3 : Bits 16-19 of the segment limit                 *ignored in long mode
	; Bit 4   : Reserved for OS use                             *ignored in long mode
	; Bit 5   : Reserved should be 0
	; Bit 6   : Segment type: 0 - 16 bits, 1 - 32 bits          *Long (L) Attribute Bit - This bit specifies that the processor is running in 64-bit mode                      
	; Bit 7   : Granularity : 0 - None, 1 - limit gets multiplied by 4k  * in long mode this must be 0
	;=======================================================================================
    .base_high:      resb  1    ;*ignored in long mode
ENDSTRUC
 
Gdt32:
    .null: 
        ISTRUC GlobalDescriptorTableEntry
            AT GlobalDescriptorTableEntry.segment_limit, dw 0
            AT GlobalDescriptorTableEntry.base_low,      dw 0
            AT GlobalDescriptorTableEntry.base_middle,   db 0
            AT GlobalDescriptorTableEntry.access,        db 0
            AT GlobalDescriptorTableEntry.granularity,   db 0
            AT GlobalDescriptorTableEntry.base_high,     db 0
        IEND
    .code_descriptor: 
        ISTRUC GlobalDescriptorTableEntry
            AT GlobalDescriptorTableEntry.segment_limit, dw 0FFFFh
            AT GlobalDescriptorTableEntry.base_low,      dw 0
            AT GlobalDescriptorTableEntry.base_middle,   db 0
            AT GlobalDescriptorTableEntry.access,        db 10011010b       ; Execute and write + DPL 0
            AT GlobalDescriptorTableEntry.granularity,   db 11001111b
            AT GlobalDescriptorTableEntry.base_high,     db 0
        IEND
    .data_descriptor: 
        ISTRUC GlobalDescriptorTableEntry
            AT GlobalDescriptorTableEntry.segment_limit, dw 0FFFFh
            AT GlobalDescriptorTableEntry.base_low,      dw 0
            AT GlobalDescriptorTableEntry.base_middle,   db 0
            AT GlobalDescriptorTableEntry.access,        db 10010010b       ; Read and write + DPL 0
            AT GlobalDescriptorTableEntry.granularity,   db 11001111b
            AT GlobalDescriptorTableEntry.base_high,     db 0
        IEND
    .end: 


;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************
 
[bits 32]
PMode32:

    break    
    break
	;-------------------------------;
	;   Set registers		;
	;-------------------------------;
    break
    break
    break

	mov		ax, 0x10		; set data segments to data selector (0x10)
	mov		ds, ax
	mov		ss, ax
	mov		es, ax
	mov		esp, 90000h		; stack begins from 90000h
	break
	break
	
	call 	cProgram 
	
    break
    break
    break

cProgram:    