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

extern void __dsk_read_byte_string(uint64_t NoBytesToRead, uint64_t PortToRead, uint8_t* Buffer); 
extern void __dsk_read_word_string(uint64_t NoWordsToRead, uint64_t PortToRead, uint16_t* Buffer); 
extern void __dsk_write_byte_string(uint64_t NoBytesToWrite, uint64_t PortToWrite, uint8_t* Buffer); 
extern void __dsk_write_word_string(uint64_t NoWordsToWrite, uint64_t PortToWrite, uint16_t* Buffer); 

extern void _IntAsmIsrCritical(void);
extern void _IntAsmIsrBasic(void);
extern void _IntAsmIsrKeyboard(void);
extern void _IntAsmIsrTimer(void);

extern void __interlock_increment_uint64t(uint64_t*);
extern void __interlock_increment_int64t(int64_t*);
extern void __interlock_increment_uint32t(uint32_t*);
extern void __interlock_increment_int32t(int32_t*);
extern void __interlock_increment_uint16t(uint16_t*);
extern void __interlock_increment_int16t(int16_t*);
extern void __interlock_increment_uint8t(uint8_t*);
extern void __interlock_increment_int8t(int8_t*);


#endif //!_ASM_DEF_H_