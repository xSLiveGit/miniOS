

%define ORIGIN_MBR 7C00h
%define ORIGIN_SSL 7E00h
%define ORIGIN_KERNEL 8000h
%define break xchg bx, bx

[bits 16]
[org ORIGIN_MBR]
;/***********************************************************************************************
;*                               1st sector                            *
;***********************************************************************************************/
BootPoint:
    
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

    jc	    .Reset					; If Carry Flag (CF) is set, there was an error. Try resetting again

    xor	    ax, ax
    mov	    es, ax
    mov     bx,  0x7E00             ;0x00:0x7e00 - ax say segment, bx say offset

; start reading sectors
.Read:
    mov	    ah, 0x02				; function 2
    mov	    al, 5					; read 2 sectors
    mov	    ch, 0					; cylinder
    mov	    cl, 2					; sector to read (The second sector)
    mov	    dh, 0					; head number
    ; mov		dl, 0					; drive number. Drive 0 is floppy drive.
    mov     dl, dl
    int	    0x13					; call BIOS - Read the sector

    jc	    .Read					; Error, so try again
    

    jmp 0:0x7E00

             
times 510 - ($-$$) db 0
db 0x55, 0xAA