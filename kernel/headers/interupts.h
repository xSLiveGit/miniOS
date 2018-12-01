#ifndef _INTERUPTS_H_
#define _INTERUPTS_H_

#include "os_definitions.h"

void IntInitializeIdt(
    PIDT    	Idt,
	PIDT_INFO	IdtDescriptor
);

void IntIdtFillEntry(
	IDT_ENTRY* 	Entry, 
	PVOID 		Isr
);

#endif // !_INTERUPTS_H_

