%define break xchg bx, bx

[bits 16]
[org 7C00h]
;/***********************************************************************************************
;*                               1st sector                            *
;***********************************************************************************************/
BootPoint:
    break
    xor 	ax, 	ax
    mov 	ds, 	ax
    mov 	es, 	ax
    mov 	gs, 	ax
    mov 	ss, 	ax
    mov 	fs, 	ax
    
    ; prepare a stack
    mov 	sp, 	BootPoint
    
; reset the boot drive
.Reset:
	mov 	ah, 0					; reset floppy disk function
    int     0x13; call BIOS
    break
    jc	    .Reset					; If Carry Flag (CF) is set, there was an error. Try resetting again

    xor	    ax, ax
    mov	    es, ax
    mov     bx,  0x7E00             ;0x00:0x7e00 - ax say segment, bx say offset

; start reading sectors
.Read:
    break
    mov	    ah, 0x02				; function 2
    mov	    al, 2					; read 2 sectors
    mov	    ch, 0					; cylinder
    mov	    cl, 2					; sector to read (The second sector)
    mov	    dh, 0					; head number
    ; mov		dl, 0					; drive number. Drive 0 is floppy drive.
    mov     dl, dl
    int	    0x13					; call BIOS - Read the sector
    break
    jc	    .Read					; Error, so try again
    
    break
    jmp 0:0x7E00

             
times 510 - ($-$$) db 'G'
db 0x55, 0xAA               ;/*End of 1st sector*/

;/***********************************************************************************************
;*                               2nd sector                            *
;***********************************************************************************************/


    ;/**********************************************************
    ;*          Setup segments and stack
    ;***********************************************************
Sector2Start:
    cli				; clear interrupts
	xor	ax, ax			; null segments
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x9000		; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	sp, 0xFFFF
	sti				; enable interrupts


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
	cli				; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax
    break
 
	jmp	08h:Stage3		; far jump to fix CS. Remember that the code selector is 0x8!



msg	db	"Installing GDT", 0
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
 
	cli			    	    ; clear interrupts
	pusha				    ; save registers
    lgdt 	[gdt_info]  ; load GDT into GDTR
	sti				; enable interrupts
	popa				; restore registers
	ret				; All done!
 
;*******************************************
; Global Descriptor Table (GDT)
;*******************************************
 
gdt_data: 
	dd 0 				; null descriptor
	dd 0 
 
; gdt code:				; code descriptor
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 				; base high
 
; gdt data:				; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 		; access
	db 11001111b 		; granularity
	db 0				; base high
 
end_of_gdt:
gdt_info: 
	dw end_of_gdt - gdt_data - 1 	    ; limit (Size of GDT)
    dd gdt_data 			            ; base of GDT

 
;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************
 
bits 32					; Welcome to the 32 bit world!
 
Stage3:

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