[ORG 0]
[BITS 16]

Setup:
   mov AX, 7E00h
   mov DS, AX
   mov ES, AX
   mov DL, 00h

   mov AH, 00h
   mov AL, 03h
   int 10h

   lea SI, [TITLE]
   call PrintS
jmp Main

Main:
.stack:             ;setup stack
   cli
   xor AX, AX
   mov DS, AX
   mov ES, AX
   mov AX, 9000h
   mov SS, AX
   mov SP, 0FFFFh
   sti

.reset:             ;load kernel to physical address 10000h 
   mov AX, 00h           
   mov DL, 00h         
   int 13h             
   jc .reset           
.read:
   mov AX, 1000h    
   mov ES, AX        
   mov BX, 00h          

   mov AH, 02h       ;start locations for reading    
   mov AL, 40h          
   mov CH, 00h         
   mov CL, 06h          
   mov DH, 00h           
   mov DL, 00h          
   int 13h             
   jc .read 

   lea SI, [KLOAD]
   call PrintS
.pMode:
   cli
   
   mov AX, 2401h     ;enable the A20 line
   int 15h
   jc Error

   pusha
   lgdt [GDTTable]   ;load a GDT
   popa

   mov EAX, CR0
   or EAX, 01h
   mov CR0, EAX
   hlt
   jmp 08h:clear_pipe

[BITS 32]
clear_pipe:
   mov AX, 10h         
   mov DS, AX              
   mov SS, AX              
   mov ESP, 90000h         
jmp Done


PrintS:
   mov AH, 0Eh
   mov BH, 00h
.loop:
   mov AL, [SI]
   cmp AL, 00h
   je .done
   int 10h
   inc SI
   jmp .loop
.done:
   ret

Error:
   cmp AH, 86h
   je .nSupport
   
.unknwon:
   lea SI, [UNERROR]
   call PrintS
   hlt
.nSupport:
   lea SI, [NOSUPPORT]
   call PrintS
   hlt

GDT:
null:      
   dd 00h
   dd 00h

code:               
   dw 0FFFFh
   dw 00h
   db 00h
   db 10011010b
   db 11001111b
   db 00h

data:               
   dw 0FFFFh
   dw 00h
   db 00h
   db 10010010b
   db 11001111b
   db 00h
GDTEnd:                

GDTTable:                      
   dw GDTEnd - GDT - 1    
   dd GDT + 0x7E000

;****************
; Strings
;****************
TITLE:
   db "-> BootSector found, second stage bootloader started", 0Ah, 0Dh, 00h
KLOAD:
   db "-> Kernel succefully loaded, setting up protected mode", 0Ah, 0Dh, 00h 
NOSUPPORT:
   db "ERROR!! - A20 line function not supported, booting halted", 0Ah, 0Dh, 00h
UNERROR:
   db "ERROR!! - An unknown error has occured, booting halted", 0Ah, 0Dh, 00h

Done:
   jmp 08h:1000h


times 2048-($-$$) db 0