#include "os_console.h"
#include "osrt.h"
#include "os_console_interpreter.h"

bool CslAcceptCommans(void)
{
    return gEnviroment.AcceptCmds;
}

void CslInit(PCONSOLE Console)
{
    os_memset(Console, sizeof(CONSOLE), 0);
    os_memcpy(Console->Buffer, CONSOLE_PREFIX, CONSOLE_PREFIX_SIZE);
    Console->BufferIdx = CONSOLE_START_IDX;
    gEnviroment.AcceptCmds = true;
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
    if(Console->BufferIdx > CONSOLE_START_IDX)
    {
        gEnviroment.AcceptCmds = false;
    }
    else
    {
        CslInit(Console);
    }
}

void CslHandleCmd(PCONSOLE Console)
{
    if(Console->BufferIdx <= CONSOLE_START_IDX)
    {
        return;
    }

    CslInterpretCmd(Console->Buffer + CONSOLE_START_IDX);    
    CslInit(Console);//where AcceptComands will be set on true :)
}