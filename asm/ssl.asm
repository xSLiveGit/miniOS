%define ORIGIN_MBR 7C00h
%define ORIGIN_SSL 7E00h
%define ORIGIN_KERNEL 8000h
%define break xchg bx, bx

%define PAG_PML4T           101000h
%define PAG_PDPT            102000h
%define PAG_PDT             103000h
%define PAG_PT              104000h

%define FIELD_OFFSET(Structure, Field) ((Field) - (Structure))

[bits 16]
[org ORIGIN_SSL]

SSLEntryPoint:
    ;/************************************************************
    ; *                     print a message
    ; ************************************************************/
  
    mov si, msg
    call StartPrint


    ;/***********************************************
    ;*                  Install GDT
    ;************************************************/
    call InstallGDT

    mov si, msg_GdtSuccess
    call    StartPrint 

    ;-------------------------------;
	;   Go into pmode		;
	;-------------------------------;

	cli				    ; clear interrupts
	mov	eax, cr0		; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax

	jmp	08h:PMode32		; far jump to fix CS. Remember that the code selector is 0x8!

msg	db	"Installing GDT", '\n', 13, 0
msg_GdtSuccess db  "GDT was successfully installed", 0
msg_32Bits db "x86 mode is on", 0

;***************************************
;	Prints a string
;	DS=>SI: 0 terminated string
;***************************************

StartPrint:

Print:
    lodsb
    or	        al, al				; al=current character
    jz	        PrintDone			; null terminator found
    mov	        ah,	0eh 			; get next character
    int	        10h
    jmp	        Print

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
;	Protected mode
;******************************************************
[bits 32]
PMode32:

    ; break    
	;-------------------------------;
	;   Set registers		;
	;-------------------------------;
    
    mov eax, FIELD_OFFSET(Gdt32, Gdt32.data_descriptor)
    mov ds, ax
    mov ss, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 
	
    ; disable pagination
    mov eax, cr0
    and eax, 7FFFh  ; Clear the PG-bit, which is bit 31
    mov cr0, eax

    ; clear the memories for tables
    mov edi, PAG_PML4T      ; Set the destination index to 0x1000.
    mov cr3, edi            ; Set control register 3 to the destination index.
    xor eax, eax            ; Nullify the A-register.
    mov ecx, 4096           ; Set the C-register to 4096.
    rep stosd               ; Clear the memory.

    ; setup tables 
    mov eax, PAG_PML4T
	mov cr3, eax            ; Set CR3 to point to PML4T address

    ; PML4T[0] = PDT
    mov eax, PAG_PDPT
    or eax, 3
    mov [PAG_PML4T], eax

    ; PAG_PDPT[0] = PAG_PDT
    mov eax, PAG_PDT
    or eax,3 
    mov [PAG_PDPT], eax

    ; PAG_PDT[0] = PAG_PT
    mov eax, PAG_PT
    or eax, 3
    mov [PAG_PDT], eax

    ; Set up indentity map - first 2 mb is identity map
    mov edi, PAG_PT
    mov ebx, 0x00000003          ; Set the B-register to 0x00000003 for access(page is present and it's writable)
    mov ecx, 512                 ; because we have 512 entries in PT

    ; PDPT[1] = PDT 
	mov dword [PAG_PDPT + 8], PAG_PDT | 011b  ; user mode / R-W / PDPT present

    ; PML4T[1] = PDPT - so 8000000000h memory points here too (512 gb)
    mov dword [PAG_PML4T + 8],  PAG_PDPT | 011b ; user mode / R-W / PDPT present

.SetEntry:
    mov DWORD [edi], ebx         ; Set the uint32_t at the destination index to the B-register.
    add ebx, 0x1000              ; Add 0x1000 to the B-register.
    add edi, 8                   ; Add eight to the destination index.
    loop .SetEntry               ; Set the next entry.
    

    mov eax, cr4                 ; Set the A-register to control register 4.
    or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 ; Set control register 4 to the A-register.

    ;;de ce trebuie intai compatibility mode si apoi pagin/protected mode???

    ; enable  compatibility mode
    mov ecx, 0xC0000080       ; Set the C-register to 0xC0000080, which is the IA32_EFER MSR.
    rdmsr
    or eax, 1 << 8            ; Enable long mode (set IA32_EFER.LME bit - bit 8)
    wrmsr      

    lgdt [Gdt64.pointer]         ; Load the 64-bit global descriptor table.

    ; break
    ; Enable paging and protected mode
    mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, (1 << 31) | (1 << 0)     ; Set the PG-bit, which is the 31nd bit, and the PM-bit, which is the 0th bit.
    mov cr0, eax                 ; Set control register 0 to the A-register.
    
    jmp FIELD_OFFSET(Gdt64, Gdt64.code_descriptor):Realm64       ; Set the code segment and enter 64-bit long mode.

; /**********************************************************************/
; /*                                GDT 64                              */
; /**********************************************************************/

Gdt64:
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
            AT GlobalDescriptorTableEntry.segment_limit, dw 0h
            AT GlobalDescriptorTableEntry.base_low,      dw 0
            AT GlobalDescriptorTableEntry.base_middle,   db 0
            AT GlobalDescriptorTableEntry.access,        db 10011100b       ; Execute and write + DPL 0
            AT GlobalDescriptorTableEntry.granularity,   db 00101111b
            AT GlobalDescriptorTableEntry.base_high,     db 0
        IEND
    .data_descriptor: 
        ISTRUC GlobalDescriptorTableEntry
            AT GlobalDescriptorTableEntry.segment_limit, dw 0h
            AT GlobalDescriptorTableEntry.base_low,      dw 0
            AT GlobalDescriptorTableEntry.base_middle,   db 0
            AT GlobalDescriptorTableEntry.access,        db 10010010b       ; Read and write + DPL 0
            AT GlobalDescriptorTableEntry.granularity,   db 00000000b
            AT GlobalDescriptorTableEntry.base_high,     db 0
        IEND
    .pointer:
        dw $ - Gdt64 - 1             ; Limit.
        dq Gdt64                     ; Base.
    .end: 

; Use 64-bit.
;******************************************************
;	                    Long mode
;******************************************************
[BITS 64]
 
Realm64:
    ; break
    cli                           ; Clear the interrupt flag.
    mov ax, FIELD_OFFSET(Gdt64, Gdt64.data_descriptor)            ; Set the A-register to the data descriptor.
    mov ds, ax                    ; Set the data segment to the A-register.
    mov es, ax                    ; Set the extra segment to the A-register.
    mov fs, ax                    ; Set the F-segment to the A-register.
    mov gs, ax                    ; Set the G-segment to the A-register.
    mov ss, ax                    ; Set the stack segment to the A-register.
    mov edi, 0xB8000              ; Set the destination index to 0xB8000.
    mov rax, 0x1F201F201F201F20   ; Set the A-register to 0x1F201F201F201F20.
    mov ecx, 500                  ; Set the C-register to 500.
   
    rep stosq                     ; Clear the screen.
    
   ; break
    call TestPaginationRoutine
    ; break
    mov rax, TestPaginationRoutine
    call rax
    break
    add rax, 40000000h
    call rax
    ; break
    jmp cProgram   

TestPaginationRoutine:
    break
    ret


cProgram:    