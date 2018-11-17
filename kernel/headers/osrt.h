#ifndef _OSRT_H_
#define _OSRT_H_

#include "screen.h"

#pragma pack(push, 1)
typedef struct _GLOBALS
{
    SCREEN_BUFFER   ScreenBuffer;
    
} GLOBALS, *PGLOBALS;
#pragma pack(pop)

extern GLOBALS gEnviroment;


#endif //!_OSRT_H_