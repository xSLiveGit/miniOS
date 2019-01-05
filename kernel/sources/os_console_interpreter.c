#include "os_console_interpreter.h"
#include "types.h"
#include "trapframe.h"
#include "os_string.h"
#include "os_timer.h"
#include "pio_mode_ata.h"
#include "os_memory.h"

#define SECTOR_CONTENT "Acesta este ultimul sector.\n Am scris ceva in ultimul sector\n. Aceasta este ultima fraza din acest sector\n"

void _HandlePrintHelpCmd()
{
    os_printf("Help:\n");
    os_printf("\t %s - help\n", CSL_COMMAND_HELP);
    os_printf("\t %s - Dump trapframe with current ctx\n", CSL_COMMAND_DUMP_TRAPFRAME);
    os_printf("\t %s - Use timer to sleep 3 seconds\n", CSL_COMMAND_DUMP_TIMER);
    os_printf("\t %s - Divide by zero: \n", CSL_COMMAND_DIVISION_BY_ZERO);
    os_printf("\t %s - Write content in last sector: \n", CSL_COMMAND_DISK_WRITE_LAST_SECTOR);
    os_printf("\t %s - Read content from last sector: \n", CSL_COMMAND_DISK_READ_LAST_SECTOR);
    os_printf("\t %s - Heap scenario 1: \n", CSL_COMMAND_HEAP_SCENARIO_1);
    os_printf("\t %s - Heap scenario 2: \n", CSL_COMMAND_HEAP_SCENARIO_2);
    os_printf("\t %s - Heap scenario 3: \n", CSL_COMMAND_HEAP_SCENARIO_3);
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


void _HandleWriteLastSector()
{
    uint8_t head = 15;
    uint8_t sector = 32;
    uint16_t cylinder = 127;
    char sectorContent[512] = { 0 };

    os_memcpy(sectorContent, SECTOR_CONTENT, sizeof(SECTOR_CONTENT));
    
    DskWrite(head, cylinder, sector, (uint8_t*)sectorContent);
}

void _HandleReadLastSector()
{
    uint8_t head = 15;
    uint8_t sector = 32;
    uint16_t cylinder = 127;
    char sectorContent[512] = { 0 };

    DskRead(head, cylinder, sector, (uint8_t*)sectorContent);   
    os_printf("%s", sectorContent);
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

void _HandleHeapScenario3(void)
{
    void* heaps[5] = { 0 };
    for(int i=0; i<3; i++)
    {
        for(int heapI = 0; heapI < 5; heapI++)
        {
            heaps[heapI] = MmAllocPage();
            os_memcpy(heaps[heapI], "ana are mere", sizeof("ana are mere"));
            os_printf("[%d]:[%x] ", heapI, heaps[heapI]);
        }

        os_printf("\n");
        
        for(int heapI = 0; heapI < 5; heapI++)
        {
            MmFreePage(heaps[heapI]);
            heaps[heapI] = NULL;
        }
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
    else if(0 == os_strcmp(Command, CSL_COMMAND_DISK_READ_LAST_SECTOR))
    {
        _HandleReadLastSector();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_DISK_WRITE_LAST_SECTOR))
    {
        _HandleWriteLastSector();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_HEAP_SCENARIO_1))
    {
        _HandleHeapScenario1();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_HEAP_SCENARIO_2))
    {
        _HandleHeapScenario2();
    }
    else if(0 == os_strcmp(Command, CSL_COMMAND_HEAP_SCENARIO_3))
    {
        _HandleHeapScenario3();
    }
    else
    {
        os_printf(" Wrong cmd. Use help\n");
    }
}

