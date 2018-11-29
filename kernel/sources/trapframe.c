#include "trapframe.h"

void TrapFrame64Dump(PTRAP_FRAME_64 TrapFrame)
{
    // char msg1[] = "RAX: %x  RSI:  %x\n";
    char msg2[] = "RBX: %x  RDI:  %x\n";
    char msg3[] = "RCX: %x  RBP:  %x\n";
    char msg4[] = "RDX: %x  Flags: %x\n";
    char msg5[] = "R8:  %x  R9:    %x\n";
    char msg6[] = "R10: %x  R11:   %x\n";
    char msg7[] = "R12: %x  R13:   %x\n";
    char msg8[] = "R14: %x  R15:   %x\n";


    os_printf("RAX: %x  RSI:  %x\n", TrapFrame->Rax, TrapFrame->Rsi);
    // os_printf(msg1, TrapFrame->Rax, TrapFrame->Rsi);
    os_printf(msg2, TrapFrame->Rbx, TrapFrame->Rdi);
    os_printf(msg3, TrapFrame->Rcx, TrapFrame->Rbp);
    os_printf(msg4, TrapFrame->Rdx, TrapFrame->Flags);
    os_printf(msg5, TrapFrame->R8,  TrapFrame->R9);
    os_printf(msg6, TrapFrame->R10, TrapFrame->R11);
    os_printf(msg7, TrapFrame->R12, TrapFrame->R13);
    os_printf(msg8, TrapFrame->R14, TrapFrame->R15);
    DebugBreak();

}
