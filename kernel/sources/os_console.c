#include "os_console.h"
#include "osrt.h"

void CslInit(PCONSOLE Console)
{
    os_memset(Console, sizeof(Console), 0);
    os_memcpy(Console->Buffer, CONSOLE_PREFIX, CONSOLE_PREFIX_SIZE);
    Console->BufferIdx = CONSOLE_START_IDX;
    os_printf("\n%s", Console->Buffer);
}

void CslWriteChr(PCONSOLE Console, char Chr)
{
    if(Console->BufferIdx >= MAX_COLUMNS)
    {
        return;
    }

    Console->Buffer[Console->BufferIdx++] = Chr;
    os_printf("%c", Chr);
}

void CslBackSpace(PCONSOLE Console)
{
    if(Console->BufferIdx <= CONSOLE_START_IDX)
    {
        return;
    }

    Console->BufferIdx--;
    ScrRemoveCharFromCurrentLine(&(gEnviroment.ScreenBuffer));
}

void CslEntr(PCONSOLE Console)
{
    CslInterpretCmd(Console->Buffer + CONSOLE_START_IDX);
    CslInit(Console);
}

void CslInterpretCmd(char* Command)
{
    os_printf("Command was: %s", Command);
}
