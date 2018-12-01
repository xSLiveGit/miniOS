#ifndef _TRAP_FRAME_H_
#define TRAP_FRAME_H_

#include "osrt.h"

#pragma pack(push, 1)
typedef struct _TRAP_FRAME_64
{
    uint64_t Rax;      
    uint64_t Rbx;      
    uint64_t Rcx;      
    uint64_t Rdx;      
    uint64_t Rsi;      
    uint64_t Rdi;      
    uint64_t Rbp;    
    uint64_t Flags;    
    uint64_t R8;       
    uint64_t R9;      
    uint64_t R10;      
    uint64_t R11;      
    uint64_t R12;      
    uint64_t R13;      
    uint64_t R14;      
    uint64_t R15;      
} _TRAP_FRAME_64, *PTRAP_FRAME_64;
#pragma pack(pop)

void TrapFrame64Dump(PTRAP_FRAME_64 TrapFrame);

# endif // ! TRAP_FRAME_H_

