#ifndef _OS_CONSOLE_H_
#define _OS_CONSOLE_H_

#include "screen.h"

#define CONSOLE_PREFIX "mini_os > "
#define CONSOLE_PREFIX_SIZE sizeof(CONSOLE_PREFIX)
#define CONSOLE_MAX_COMMAND MAX_COLUMNS - CONSOLE_PREFIX_SIZE
#define CONSOLE_START_IDX CONSOLE_PREFIX_SIZE

typedef struct _CONSOLE
{
    char        Buffer[CONSOLE_MAX_COMMAND];
    uint8_t     BufferIdx;     
} CONSOLE, *PCONSOLE; 

void CslInit(PCONSOLE);
void CslWriteChr(PCONSOLE Console, char Chr);
void CslBackSpace(PCONSOLE Console);
void CslEntr(PCONSOLE Console);
void CslInterpretCmd(char*);

#endif //! _OS_CONSOLE_H_