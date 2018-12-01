#ifndef _ASM_DEF_H_
#define _ASM_DEF_H_

#include "interupts.h"
#include "trapframe.h"

extern void DebugBreak(void);
extern void __hlt(void);
extern void __sti(void);
extern void __cli(void);
extern void __lidt(void*);
extern void __outb(uint16_t Port, uint8_t Data);
extern uint8_t __inb(uint16_t Port);
extern void __load_trap_frame(PTRAP_FRAME_64 TrapFrame);

extern void _IntAsmCritical(void);
extern void _IntAsmBasic(void);
extern void _IntAsmIsrKeyboard(void);
// extern void IntAsmTrapFrame(void);
// extern void IntAsmLidt(PIDT_INFO);

#endif //!_ASM_DEF_H_