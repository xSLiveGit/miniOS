#include "interupts.h"
#include "asm_def.h"
#include "osrt.h"
#include "os_keyboard.h"
#include "pic.h"

void IntPrvFillIdt(PIDT Idt);

void IntRemapPic(void)
{
	//; Setup to initialize the primary PIC. Send ICW 1
    //; Bit 0 - Set to 1 so we can sent ICW 4
    //; Bit 1 - PIC cascading bit. x86 architectures have 2 PICs, so we need the primary PIC cascaded with the slave. Keep it 0
    //; Bit 2 - CALL address interval. Ignored by x86 and kept at 8, so keep it 0
    //; Bit 3 - Edge triggered/Level triggered mode bit. By default, we are in edge triggered, so leave it 0
    //; Bit 4 - Initialization bit. Set to 1
    //; Bits 5...7 - Unused on x86, set to 0.

    // ; Send ICW1
    __outb(PIC_MASTER_CTRL, ICW_1);
    __outb(PIC_SLAVE_CTRL, ICW_1); // ; Remember that we have 2 PICs. Because we are cascading with this second PIC, send ICW 1 to second PIC command register

    // ; Send ICW2 - used to map the base address of the IVT of which the PIC are to use.
    __outb(PIC_MASTER_DATA, IRQ_0);
    __outb(PIC_SLAVE_DATA, IRQ_8); //Secondary PIC handles IRQ's 8..15. IRQ 8 is now mapped to use interrupt 0x28

    //; ICW3
    __outb(PIC_MASTER_DATA, ICW_3_MASTER);
    __outb(PIC_SLAVE_DATA, ICW_3_SLAVE);
    //; both PICs are connected to use IR line 2 to communicate with each other. We have also set a base interrupt number for both PICs to use.
    
    //; ICW4
    //; Initialization Control Word (ICW) 4
    //; Bit Number	Value	Description
    //; 0	uPM	If set (1), it is in 80x86 mode. Cleared if MCS-80/86 mode
    //; 1	AEOI	If set, on the last interrupt acknowledge pulse, controller automatically performs End of Interrupt (EOI) operation
    //; 2	M/S	Only use if BUF is set. If set (1), selects buffer master. Cleared if buffer slave.
    //; 3	BUF	If set, controller operates in buffered mode
    //; 4	SFNM	Special Fully Nested Mode. Used in systems with a large amount of cascaded controllers.
    //; 5-7	0	Reserved, must be 0
    __outb(PIC_MASTER_DATA, 1); // bit 0 enables 80x86 mode
    __outb(PIC_SLAVE_DATA, 1); // bit 0 enables 80x86 mode
}


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
    IntRemapPic();
    // IntInitPic();

    DebugBreak();
    __cli();
    __lidt(IdtDescriptor); //extern void IntAsmLidt(PIDT_INFO);
    __sti();
}

void IsrBasic(void)
{
    __cli();
    __outb(PIC_MASTER_CTRL, PIC_EOI);
    __sti();
}

void IntPrvFillIdt(PIDT Idt)
{
    for(size_t i=0; i< IDT_NO_CRITICAL_ENTRIES; i++)
    {
        IntIdtFillEntry(&(Idt->Entries[i]), &_IntAsmCritical);
    }

    for(size_t i=IDT_NO_CRITICAL_ENTRIES; i<IDT_NO_ENTRIES; i++)
    {
        IntIdtFillEntry(&(Idt->Entries[i]), &_IntAsmBasic);
    }
        
    IntIdtFillEntry(&(Idt->Entries[33]), &_IntAsmIsrKeyboard);
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

void IsrCritical(void)
{
    __cli();
    TRAP_FRAME_64 frame = {0};
    
    __load_trap_frame(&frame);
    TrapFrame64Dump(&frame);

    __outb(PIC_MASTER_CTRL, PIC_EOI);
    
    __hlt();

    __sti();
}