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
