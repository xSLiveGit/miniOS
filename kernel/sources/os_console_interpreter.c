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
    os_printf("Am primit un pointer\n");
    if(NULL == mmPage)
    {
        os_printf("MmAllocPage returned nullptr");
        return;
    }

    os_printf("Allocated page: { %x }", mmPage);
    __debugbreak();
    __debugbreak();
    
    for(int i=0; i< 2; i++)
    {
        ((char*)mmPage)[i] = heapMsg[i];
    }

    os_printf("Allocated page: { %x }", mmPage);
    os_printf("o SA CURAT");
    MmFreePage(mmPage);
    os_printf("Am curatat");
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

