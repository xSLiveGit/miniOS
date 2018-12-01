#ifndef _ASM_DEF_H_
#define _ASM_DEF_H_

#include "interupts.h"

extern void DebugBreak(void);
extern void __hlt(void);

extern void AsmIntDumpTrapFrame(void);
extern void IntAsmKeyboard(void);
extern void IntInitPic(void);
extern void IntAsmBasic(void);
extern void IntAsmTrapFrame(void);
extern void IntAsmLidt(PIDT_INFO);




#endif //!_ASM_DEF_H_