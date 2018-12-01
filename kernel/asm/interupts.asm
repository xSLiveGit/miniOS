
%include "trapframe.inc"
%include "interupts.inc"
%include "utils.inc"
%include "keyboard.inc"



GLOBAL _IntAsmIsrBasic
extern IsrBasic
; void _IntAsmIsrBasic(void)
_IntAsmIsrBasic:
	call IsrBasic
	iretq 



GLOBAL _IntAsmIsrCritical
extern IsrCritical;
; void _IntAsmIsrCritical(void)
_IntAsmIsrCritical:
	call IsrCritical
	iretq



GLOBAL _IntAsmIsrKeyboard
extern IsrKeyboard
; void _IntAsmIsrKeyboard(void)
_IntAsmIsrKeyboard:
	call IsrKeyboard
	iretq



GLOBAL _IntAsmIsrTimer
extern IsrTimer;
; void _IntAsmIsrTimer(void)
_IntAsmIsrTimer:
	call IsrTimer
	iretq