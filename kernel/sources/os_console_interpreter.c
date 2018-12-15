#include "os_console_interpreter.h"
#include "types.h"
#include "trapframe.h"
#include "os_string.h"
#include "os_timer.h"
#include "os_memory.h"

void _HandlePrintHelpCmd()
{
    os_printf("Help:\n");
    os_printf("\t %s - help\n", CSL_COMMAND_HELP);
    os_printf("\t %s - Dump trapframe with current ctx\n", CSL_COMMAND_DUMP_TRAPFRAME);
    os_printf("\t %s - Use timer to sleep 3 seconds\n", CSL_COMMAND_DUMP_TIMER);
    os_printf("\t %s - Divide by zero: \n", CSL_COMMAND_DIVISION_BY_ZERO);
    os_printf("\t %s - Heap scenario 1: \n", CSL_COMMAND_HEAP_SCENARIO_1);
}

void _HandleTimeoutCmd()
{
    os_printf("I will wait for 3 secounds\n");
    TimerSleep(3000000);
    os_printf("Timeout :)\n");
}

void _HandleTrapframeCmd()
{
    TrapFrameDump();
}

void _HandleDivisionByZero()
{
    int zero = 1;
    zero--; //use this way because compiler does not let me to divide by zero :) 

    int result = zero / zero;
    os_printf("No result: %d", result);//due compiler
}

void _HandleHeapScenario1()
{
    char heapMsg[] = "Am scris ceva in heap";
    void* mmPage = MmAllocPage();
    if(NULL == mmPage)
    {
        os_printf("MmAllocPage returned nullptr");
        return;
    }

    for(int i=0; i< sizeof(heapMsg); i++)
    {
        ((char*)mmPage)[i] = heapMsg[i];
    }

    os_printf("Allocated page: { %x }, Content: { %s }", mmPage, (char*)mmPage);
    MmFreePage(mmPage);
}

void CslInterpretCmd(char* Command)
{
    if(0 == os_strcmp(Command, CSL_COMMAND_HELP))
    {
        _HandlePrintHelpCmd();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_DUMP_TRAPFRAME))
    {
        _HandleTrapframeCmd();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_DUMP_TIMER))
    {
        _HandleTimeoutCmd();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_DIVISION_BY_ZERO))
    {
        _HandleDivisionByZero();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_HEAP_SCENARIO_1))
    {
        _HandleHeapScenario1();
    }
    else
    {
        os_printf(" Wrong cmd. Use help\n");
    }
}

