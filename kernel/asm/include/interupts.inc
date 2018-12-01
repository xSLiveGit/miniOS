
%define ICW_1 0x11				        ; 00010001 binary. Enables initialization mode and we are sending ICW 4

%define ICW_3_MASTER    0x4		        ; 0x04 => 0100, second bit (IR line 2)
%define ICW_3_SLAVE     0X2		        ; 010=> IR line 2

%define PIC_MASTER_CTRL 0x20			; Primary PIC control register
%define PIC_SLAVE_CTRL 0xA0				; Secondary PIC control register
 
%define PIC_MASTER_DATA 0x21			; Primary PIC data register
%define PIC_SLAVE_DATA 0xA1				; Secondary PIC data register
 
%define IRQ_0	0x20				; IRQs 0-7 mapped to use interrupts 0x20-0x27
%define IRQ_8	0x28				; IRQs 8-15 mapped to use interrupts 0x28-0x36

%define PIC_EOI		    0x20	
