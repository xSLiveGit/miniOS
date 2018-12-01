#ifndef _OSRT_H_
#define _OSRT_H_

#include "screen.h"
#include "asm_def.h"
#include "globals.h"

bool InitEnviroment(); //true means success

void os_memset(void* Destionation, size_t Size, uint8_t Byte);
void os_memcpy(void* Destionation, void* Source, size_t Size);

GLOBALS gEnviroment;

void PrvPrintString(char* Buffer);
void os_printf(
    char*   Format
    , ...
);


#endif //!_OSRT_H_