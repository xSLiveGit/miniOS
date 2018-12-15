#include "trapframe.h"
#include "os_definitions.h"
#include "asm_def.h"

void TrapFrame64Dump(PTRAP_FRAME_64 TrapFrame)
{
    os_printf("RAX: %x  RSI:    %x\n", TrapFrame->Rax, TrapFrame->Rsi);
    os_printf("RBX: %x  RDI:    %x\n", TrapFrame->Rbx, TrapFrame->Rdi);
    os_printf("RCX: %x  RBP:    %x\n", TrapFrame->Rcx, TrapFrame->Rbp);
    os_printf("RDX: %x  Flags:  %x\n", TrapFrame->Rdx, TrapFrame->Flags);
    os_printf("R8:  %x  R9:     %x\n", TrapFrame->R8,  TrapFrame->R9);
    os_printf("R10: %x  R11:    %x\n", TrapFrame->R10, TrapFrame->R11);
    os_printf("R12: %x  R13:    %x\n", TrapFrame->R12, TrapFrame->R13);
    os_printf("R14: %x  R15:    %x\n", TrapFrame->R14, TrapFrame->R15);
}

void TrapFrameDump()
{
    TRAP_FRAME_64 frame = {0};
    __load_trap_frame(&frame);
    TrapFrame64Dump(&frame);
}