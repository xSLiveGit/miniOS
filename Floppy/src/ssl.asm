%define break xchg bx, bx

[org 0x7E00]
[bits 16]
EntryPoint:
    ;break 

;    ;DO 16 bits stuff
 
;    ; change mode to video
;mov ah, 0		
;mov al, 13h
;int 10h
;
;mov ax, 0xA000
;mov fs, ax
;
;;320 x 200 / 256 colors
;xor di, di		;y=0
;for1:
;	xor si, si	;x=0
;	for2:
;		
;		mov ax, si
;		xor ax, di
;		
;		push ax
;		push di
;		push si
;		call putpixel
;		
;		inc si
;		cmp si, 320
;		jb for2
;		
;	inc di
;	cmp di, 200
;	jb for1
;
jmp Trecere
    
;;putpixel(x,y,c) -> 320*y + x]
;putpixel:
;	push bp
;	mov bp, sp
;	pusha
;	
;	mov bx, 320
;	mov ax, [bp + 6]	;y
;	mul bx				;dx:ax=320*y
;	
;	add ax, [bp + 4]	;+x
;	
;	mov cl, [bp + 8]	;culoare
;	
;	mov bx, ax
;	mov [fs:bx], cl		; setam culoarea la adresa calculata
;	
;	popa
;	pop bp
;	ret (3*2)
    
Trecere:
	; RM to PM32 transition
	cli
    
	lgdt 	[Gdt]
	mov 	eax, 	cr0
	or 		al, 	1
	mov 	cr0, 	eax
	; suntem in protected mode
	jmp		8: .bits32

.bits32:
	[bits 32]
	mov		ax,		16
	mov		ds,		ax
	mov		[ds:0xb8000], BYTE 'H'
    
    ;DO 32 bits stuff
	

backTo16:
	; PM32 to RM transition
	mov		ax,		32
	mov		ds,		ax
	jmp		24:.bits16
.bits16:
	[bits 16]

	mov		eax,	cr0
	and		eax,	0xFFFFFFFF - 1
	mov		cr0,	eax
	jmp		00:.seg0
.seg0:
	xor		ax,		ax
	mov		ds,		ax
    mov		es,		ax

	sti
    
;change to text mode
mov ah, 0		;video mode
mov al, 3		;text
int 10h
    
    ;afisam un string pe ecran
    mov bp, string	;es:bp -> stringul
    xor dx, dx
    mov ah, 13h
    mov cx, len
    mov al, 0
    mov bl, 7
    int 10h    
	
test:
    jmp test
	
string: db "Suntem in 16 biti."
len equ ($-string)    
;
; Data
;
Gdt:
	.limit		dw	GdtTable.end - GdtTable - 1
	.base		dd  GdtTable

FLAT_DESCRIPTOR_CODE32  equ 0x00CF9A000000FFFF      ; Code: Execute/Read
FLAT_DESCRIPTOR_DATA32  equ 0x00CF92000000FFFF      ; Data: Read/Write
FLAT_DESCRIPTOR_CODE16  equ 0x00009B000000FFFF      ; Code: Execute/Read, accessed
FLAT_DESCRIPTOR_DATA16  equ 0x000093000000FFFF      ; Data: Read/Write, accessed
    
GdtTable:
	.null		dq 0
	.code32		dq FLAT_DESCRIPTOR_CODE32
	.data32		dq FLAT_DESCRIPTOR_DATA32
	.code16		dq FLAT_DESCRIPTOR_CODE16
	.data16		dq FLAT_DESCRIPTOR_DATA16
	.end:
	
