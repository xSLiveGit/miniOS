%include "utils.inc"

GLOBAL __debugbreak
; void __debugbreak(void)
__debugbreak:
    push rbp
    mov rbp, rsp

    xchg bx, bx

    pop rbp
    ret


GLOBAL __hlt
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


GLOBAL __sti
; void __sti(void);
__sti:
    sti
    ret


GLOBAL __cli
; void __cli(void);
__cli:
    cli
    ret


GLOBAL __lidt
; void __lidt(PVOID)
__lidt:
    lidt    [rcx]
    ret


GLOBAL __outb
; void __outb(uint16_t Port, uint8_t Data);
__outb:
    mov     rax,    rdx
    mov     rdx,    rcx
    out     dx,     al 
    ret


GLOBAL __inb
; uint8_t __inb(uint16_t Port);
__inb:
    mov     dx,     cx
    in      al,     dx
    ret


GLOBAL __dsk_read_byte_string
; void __dsk_read_byte_string(uint64_t NoBytesToRead, uint64_t PortToRead, uint8_t* Buffer); 
__dsk_read_byte_string:
; NoWordToRead - Rcx 
; PortToRead - Rdx
; Buffer - R8
    xchg bx, bx
    push rsi
    mov rsi, r8
    rep insb
    pop rsi
    ret


GLOBAL __dsk_read_word_string
; void __dsk_read_word_string(uint64_t NoWordToRead, uint64_t PortToRead, uint16_t* Buffer); 
__dsk_read_word_string:
; NoWordToRead - Rcx 
; PortToRead - Rdx
; Buffer - R8
    push rsi
    mov rsi, r8
    rep insw
    pop rsi
    ret


GLOBAL __dsk_write_byte_string
; void __dsk_write_byte_string(uint64_t NoBytesToWrite, uint64_t PortToWrute, uint8_t* Buffer); 
__dsk_write_byte_string:
; NoBytesToWrite - Rcx 
; PortToWrute - Rdx
; Buffer - R8
    xchg bx, bx
    push rsi
    mov rsi, r8
    rep outsb
    pop rsi
    ret


GLOBAL __dsk_write_word_string
; void __dsk_write_word_string(uint64_t NoWordToWrite, uint64_t PortToWrute, uint16_t* Buffer); 
__dsk_write_word_string:
; NoWordToWrite - Rcx 
; PortToWrute - Rdx
; Buffer - R8
    push rsi
    mov rsi, r8
    rep outsw
    pop rsi
    ret



GLOBAL __interlock_increment_uint64t
; void __interlock_increment_uint64t(uint64t*)
__interlock_increment_uint64t:
lock inc QWORD [rcx]
ret


GLOBAL __interlock_increment_int64t
; void __interlock_increment_uint32t(int64t*)
__interlock_increment_int64t:
lock inc QWORD [rcx]
ret


GLOBAL __interlock_increment_uint32t
; void __interlock_increment_uint32t(uint32t*)
__interlock_increment_uint32t:
lock inc DWORD [rcx]
ret


GLOBAL __interlock_increment_int32t
; void __interlock_increment_int32t(int32t*)
__interlock_increment_int32t:
lock inc DWORD [rcx]
ret


GLOBAL __interlock_increment_uint16t
; void __interlock_increment_uint16t(uint16_t*)
__interlock_increment_uint16t:
lock inc WORD [rcx]
ret


GLOBAL __interlock_increment_int16t
; void __interlock_increment_int16t(int16_t*)
__interlock_increment_int16t:
lock inc WORD [rcx]
ret


GLOBAL __interlock_increment_uint8t
; void __interlock_increment_uint8t(uint8_t*)
__interlock_increment_uint8t:
lock inc BYTE [rcx]
ret


GLOBAL __interlock_increment_int8t
; void __interlock_increment_int8t(int8_t*)
__interlock_increment_int8t:
lock inc BYTE [rcx]
ret






GLOBAL DiskIOSector
; void DiskIOSector(WORD Cylinder, BYTE SectorIndex, BYTE Head, BYTE* OutputBuffer, BYTE Command)
DiskIOSector:
	push rbp 
	mov rbp, rsp 
	PUSH_A

	; Cylinder     = RCX
	; SectorIndex  = RDX
	; Head         = R8
	; OutputBuffer = R9
	; Command      = [rbp + 16]
	
	mov rbx, rdx   ; we will use dx for port register
	xor rdx, rdx

	; DRIVE/HEAD register  port:  0x1F6
	; |1|x|1|x|x|x|x|x|
	;  | | | | +---------- HEAD      - is the 4-bit address used to select the head
	;  | | | |------------ DRV       - the bit used to select the drive. Master is 0. Slave is 1
	;  | | |-------------- RESERVED  - must be 1
	;  | |---------------- LBA       - LBA (logical block address) for 1, CHS for 0
	;  |------------------ RESERVED  - must be 1
	mov rax, r8         
	or  al,  10100000b   ; Master DRV | CHS mode 
	mov dx,  1F6h
	out dx,  al 

	; Write any required parameters to the Features, Sector Count, Sector Number, Cylinder and Drive/Head registers.

	; SECTOR COUNT register  port:  0x1F2
	; This register specifies the number of sectors of data to be transferred during read/write sector commands
	mov al, 1
	mov dx, 1F2h
	out dx, al
	
	; SECTOR NUMBER register  port:  0x1F3
	; This register contains the starting sector number for any disc access
	mov rax, rbx 
	mov dx,  1F3h
	out dx,  al

	; CYLINDER LOW register port:  0x1F4
	; This register contains the eight least significant bits of the starting cylinder address for any disc access
	mov rax, rcx
	mov dx,  1F4h
	out dx,  al

	; CYLINDER HIGH register port:  0x1F5
	; This register contains the most significant bits of the starting cylinder address for any disc access
	mov  rax, rcx
	xchg al,  ah
	mov  dx,  1F5h
	out  dx,  al

	; COMMAND register  port:  0x1F7        !WRITE ONLY!
	; This eight-bit register contains the host command. When this register is written, the drive immediately begins executing the command
	mov rax, [rbp + 16] 
	mov dx, 1F7h
	out dx, al

	; Now The drive sets BSY and prepares for data transfer. When the drive is ready to accept a block of data, it sets DRQ and clears BSY. When the host detects DRQ is set to 1, the host writes one block of data from the Data register.

	; STATUS register  port:  0x1F7         !READ ONLY! 
	; |x|x|x|x|x|x|x|x|
	;  | | | | | | | +---- ERR      - error bit. It is set to 1 when the previous command ended in some type of error
	;  | | | | | | |------ IDX      - index bit. This bit usually contains a 0, except once per disc revolution when it is toggled from 0 to 1 and back to 0
	;  | | | | | |-------- CORR     - corrected data bit. It is set to 1 when a correctable data error has been encountered and the data has been corrected
	;  | | | | |---------- DRQ      - data request bit. It is set to 1 when the drive is ready to transfer a word or byte of data between the host and the data port
	;  | | | |------------ DSC      - drive seek complete bit. It is set to 1 when the disc drive is not seeking
	;  | | |-------------- DWF      - drive write fault bit. When there is a write fault error, this bit is set to 1 and is not changed until the Status register is read by the host, at which time the bit again indicates the current write fault status.
	;  | |---------------- DRDY     - drive ready indicator bit. This bit is set to 0 at power up and remains set at 0 until the drive is ready to accept a command.
	;  |------------------ BSY      - is the busy bit. It is set to 1 whenever the drive has access to the command block
	; This register contains either the drive status or the controller status. It is updated at the completion of each command
	
	; Wait for completion
	.checkBusy:
	mov  dx, 1F7h 
	in   al, dx 
	test al, 8
	jz .checkBusy


	POP_A
	leave
	ret 


GLOBAL DiskReadBuffer 
; void DiskReadBuffer(uint16_t Cylinder, uint8_t SectorIndex, uint8_t Head, uint8_t* InputBuffer)
DiskReadBuffer:
    PUSH_A
	mov rcx, 256    ; no words read
	mov rdx, 1F0h   
	mov rdi, r9     ; destination bufer
	rep insw        ; input from port to string 
    POP_A
    ret

GLOBAL DiskWriteBuffer 
; void DiskWriteBuffer(uint16_t Cylinder, uint8_t SectorIndex, uint8_t Head, uint8_t* InputBuffer)
DiskWriteBuffer:
    PUSH_A
	mov rcx, 256    ; no words read
	mov rdx, 1F0h   
	mov rdi, r9     ; destination bufer
	rep insw        ; input from port to string 
    POP_A
    ret