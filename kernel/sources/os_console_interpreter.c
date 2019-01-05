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
    os_printf("\t %s - Heap scenario 2: \n", CSL_COMMAND_HEAP_SCENARIO_2);
}

void _HandleTimeoutCmd()
{
    os_printf("I will wait for 3 secounds\n");
    TimerSleep(3000);
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
    os_printf("Mm:{%x}. Content: {%s}\n", mmPage, mmPage);

    MmFreePage(mmPage);
}

void _HandleHeapScenario2(void)
{
    void* heaps[64] = { 0 };
    int mod=1;
    for(int i=0; i<64; i++)
    {
        heaps[i]  = MmAllocPage(); 
        
        os_printf("[%x]", heaps[i]);
        if(mod % 4 == 0)
        {
            os_printf("\n");            
        }

        mod = (mod+1) % 4;
    }

    __debugbreak();
    void* shouldBeNull = MmAllocPage();
    os_printf("65th allocation is: [%x]", shouldBeNull);
    
    MmFreePage(heaps[63]);
    heaps[63] = NULL;

    void* shouldntBeNull = MmAllocPage();
    os_printf("66th allocation is: [%x]", shouldntBeNull);
    
    MmFreePage(shouldntBeNull);
    for(int i=0; i<63; i++)
    {
        MmFreePage(heaps[i]);
        heaps[i] = NULL;
    }
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
    else if(0 == os_strcmp(Command, CSL_COMMAND_HEAP_SCENARIO_2))
    {
        _HandleHeapScenario2();
    }
    else
    {
        os_printf(" Wrong cmd. Use help\n");
    }
}

