#ifndef _INTERUPTS_H_
#define _INTERUPTS_H_

#include "types.h"

#define IDT_NO_ENTRIES 256

#define GET_INT_OFFSET_LOW(ISR) (uint16_t)(((uint64_t)ISR) & 0xFFFF)
#define GET_INT_OFFSET_MID(ISR) (uint16_t)((((uint64_t)ISR) & (0xFFFF >> 8)) >> 16)
#define GET_INT_OFFSET_HIGH(ISR) (uint32_t)((((uint64_t)ISR) & (0xFFFFFFFF >> 16)) >> 32)


#pragma pack(push, 1)
typedef struct _IDT_ENTRY
{
   uint16_t OffsetLow; 	// offset bits 0..15
   uint16_t Selector; 	// a code segment selector in GDT or LDT
   uint8_t 	Ist;       	// bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t 	Flags; 		// type and attributes
   uint16_t OffsetMid; 	// offset bits 16..31
   uint32_t OffsetHigh; // offset bits 32..63
   uint32_t Zero;     	// reserved
} IDT_ENTRY, *PIDT_ENTRY;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct _IDT
{
	IDT_ENTRY Entries[IDT_NO_ENTRIES];
} IDT, *PIDT;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct _IDT_INFO
{
	uint16_t IdtSize;
	PIDT     IdtBase;
} IDT_INFO, *PIDT_INFO;
#pragma pack(pop)


void 
IntInitializeIdt(
    PIDT    	Idt,
	PIDT_INFO	IdtDescriptor
);

void IntIdtFillEntry(
	IDT_ENTRY* 	Entry, 
	PVOID 		Isr
);

#endif // !_INTERUPTS_H_

