%macro PUSH_A 0
    push rax 
    push rbx 
    push rcx 
    push rdx 
    push rsi 
    push rdi 
    push rbp 
    push r8 
    push r9
    push r10 
    push r11 
    push r12 
    push r13 
    push r14
    push r15 
    pushf
%endmacro

%macro POP_A 0
    popf
    pop r15
    pop r14 
    pop r13 
    pop r12 
    pop r11 
    pop r10 
    pop r9 
    pop r8 
    pop rbp 
    pop rdi 
    pop rsi 
    pop rdx 
    pop rcx 
    pop rbx 
    pop rax 
%endmacro


[Bits 64]
GLOBAL __debugbreak
; void __debugbreak(void)
__debugbreak:
    push rbp
    mov rbp, rsp

    xchg bx, bx

    pop rbp
    ret

[Bits 64]
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


[Bits 64]
GLOBAL __sti
; void __sti(void);
__sti:
    sti
    ret


[Bits 64]
GLOBAL __cli
; void __cli(void);
__cli:
    cli
    ret


[Bits 64]
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


GLOBAL __dsk_write_sector
; void __dsk_write_sector(uint8_t* Buffer);
__dsk_write_sector:
	PUSH_A
	mov rsi, rcx
	mov rcx, 256
	mov rdx, 1F0h
	rep outsw
	POP_A
	ret


GLOBAL __dsk_read_sector
; void __dsk_read_sector(uint8_t* Buffer);
__dsk_read_sector:
; RCX - Buffer
	PUSH_A
	mov rdi, rcx
	mov rcx, 256
	mov rdx, 1F0h
	rep insw
	POP_A
	ret

GLOBAL __invlpg
; void __invlpg(void* Address);
__invlpg:
; Rcx - Address
	invlpg [rcx]	
	ret
