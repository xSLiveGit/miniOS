#include "interupts.h"
#include "asm_def.h"
#include "osrt.h"

void IntPrvFillIdt(PIDT Idt);

void 
IntInitializeIdt(
    PIDT    	Idt,
	PIDT_INFO	IdtDescriptor
)
{
    os_printf("Initializez pic-ul");

    IdtDescriptor->IdtBase = Idt;
    IdtDescriptor->IdtSize = sizeof(*Idt) - 1;

    IntPrvFillIdt(Idt);
    
    os_printf("Idt addr: %x\n", Idt);
    os_printf("Idt descritpr addr: %x\n", IdtDescriptor);
    IntInitPic();

    DebugBreak();
    IntAsmLidt(IdtDescriptor); //extern void IntAsmLidt(PIDT_INFO);
}

void IntPrvFillIdt(PIDT Idt)
{
    os_printf("Fill idt");
    os_printf("IntAsmBasic addr: %x\n", (uint64_t)(IntAsmBasic));
    // IntIdtFillEntry(&(Idt->Entries[0]), &IntAsmTrapFrame);

    for(size_t i=0; i<IDT_NO_ENTRIES; i++)
    {
        IntIdtFillEntry(&(Idt->Entries[i]), &IntAsmBasic);
    }
        
    IntIdtFillEntry(&(Idt->Entries[33]), &IntAsmKeyboard);
    
    uint64_t addr = (uint64_t)(((uint64_t)Idt->Entries[0].OffsetHigh << 32) + ((uint64_t)Idt->Entries[0].OffsetMid << 16) + ((uint64_t)Idt->Entries[0].OffsetLow));
    os_printf("IntAsmBasic addr reconstruction: %x\n", (uint64_t)(addr));

}

void IntIdtFillEntry(
	IDT_ENTRY* 	Entry,
	PVOID 		Isr
)
{
    Entry->OffsetHigh   = (((uint64_t)Isr) & 0xFFFFFFFF00000000) >> 32; //GET_INT_OFFSET_HIGH(Isr);
    Entry->OffsetMid    = (((uint64_t)Isr) & 0xFFFF0000) >> 16; // GET_INT_OFFSET_MID(Isr);
    Entry->OffsetLow    = (((uint64_t)Isr) & 0xFFFF); //GET_INT_OFFSET_LOW(Isr);
    Entry->Zero         = 0;
    Entry->Ist          = 0;
    Entry->Selector     = 0x8;  
    Entry->Flags        = 0x8E;// P = 1b; DPL = 00b; Reserved = 0b; type = 1110b -> code | conforming | executable and readable | not accessed
}