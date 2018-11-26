#ifndef _OSRT_H_
#define _OSRT_H_

#include "screen.h"
#include "asm_def.h"
// #include "os_utils.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

#pragma pack(push, 1)
typedef struct _GLOBALS
{
    SCREEN_BUFFER   ScreenBuffer;
} GLOBALS, *PGLOBALS;
#pragma pack(pop)

bool InitEnviroment(); //trus means success
void os_memset(void* Destionation, size_t Size, uint8_t Byte);

GLOBALS gEnviroment;

void PrvPrintString(char* Buffer);
void os_printf(
    char*   Format
    , ...
);


#endif //!_OSRT_H_