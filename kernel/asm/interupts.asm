
%include "trapframe.inc"
%include "interupts.inc"
%include "utils.inc"
%include "keyboard.inc"

GLOBAL _IntAsmBasic
GLOBAL _IntAsmCritical
GLOBAL _IntAsmIsrKeyboard

extern IsrCritical;
extern IsrKeyboard
extern IsrBasic


; void IntAsmBasic(void)
_IntAsmBasic:
	call IsrBasic
	iretq 


; void IntAsmCritical(void)
_IntAsmCritical:
	call IsrCritical
	iretq


; void IntAsmIsrKeyboard(void)
_IntAsmIsrKeyboard:
	call IsrKeyboard
	iretq