#ifndef _OSRT_H_
#define _OSRT_H_

#include "screen.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

#pragma pack(push, 1)
typedef struct _GLOBALS
{
    SCREEN_BUFFER   ScreenBuffer;
} GLOBALS, *PGLOBALS;
#pragma pack(pop)

bool InitEnviroment();//trus means success
void os_memset(void* Destionation, size_t Size, uint8_t Byte);

GLOBALS gEnviroment;


#endif //!_OSRT_H_