#ifndef _ASM_DEF_H_
#define _ASM_DEF_H_

#include "types.h"
#include "interupts.h"
#include "trapframe.h"

extern void __debugbreak(void);
extern void __load_trap_frame(PTRAP_FRAME_64 TrapFrame);

extern void __hlt(void);
extern void __sti(void);
extern void __cli(void);
extern void __lidt(void*);
extern void __outb(uint16_t Port, uint8_t Data);
extern uint8_t __inb(uint16_t Port);

extern void _IntAsmIsrCritical(void);
extern void _IntAsmIsrBasic(void);
extern void _IntAsmIsrKeyboard(void);
extern void _IntAsmIsrTimer(void);

extern void __interlock_increment_uint64t(volatile uint64_t*);
extern void __interlock_increment_int64t(volatile int64_t*);
extern void __interlock_increment_uint32t(volatile uint32_t*);
extern void __interlock_increment_int32t(volatile int32_t*);
extern void __interlock_increment_uint16t(volatile uint16_t*);
extern void __interlock_increment_int16t(volatile int16_t*);
extern void __interlock_increment_uint8t(volatile uint8_t*);
extern void __interlock_increment_int8t(volatile int8_t*);

extern void __dsk_read_sector(uint8_t* Buffer);
extern void __dsk_write_sector(uint8_t* Buffer);



#endif //!_ASM_DEF_H_