#include "os_timer.h"
#include "os_definitions.h"
#include "asm_def.h"
#include "types.h"
#include "osrt.h"

uint64_t gTickCount;
bool dumpTickCount;

void TimerInit(void)
{
    dumpTickCount = false;
    uint32_t desiredFreq = 100;
    uint32_t timerBaseFreq = 1193180;  //Magic value from https://wiki.osdev.org/Programmable_Interval_Timer :) 
    uint16_t divisor = timerBaseFreq / desiredFreq;

    uint8_t lowByte = ((uint8_t*)(&divisor))[0];
    uint8_t highByte = ((uint8_t*)(&divisor))[1];

    __outb(TIMER_COMMAND_PORT, 0x36); //SQUARE-WAVE mode | 16 bit counter | lobyte and hibyte
    __outb(TIMER_CHANNEL0_PORT, lowByte);
    __outb(TIMER_CHANNEL0_PORT, highByte);
}

void IsrTimer(void)
{
    __cli();

    __interlock_increment_uint64t(&gTickCount);
    __outb(PIC_MASTER_CTRL, PIC_EOI);

    if(dumpTickCount)
        os_printf("IsrTime was called. New tickCountValue is: { %x }\n", gTickCount);
    __sti();
}

uint64_t TimerGetTickCount(void)
{
    return gTickCount;
}

void TimerSleep(uint64_t Milliseconds)
{
    os_printf("TimerSleep\n");
    dumpTickCount = true;
    __debugbreak();
    uint64_t intialTimerTickCount = TimerGetTickCount();
    uint64_t period = 100;
    // os_printf(  
    //     "Initial tick count:{ %x } and miliseconds: { %x } \n", 
    //     intialTimerTickCount, 
    //     Milliseconds
    // );
    
    while(true)
    {
        uint64_t currentTickCout = TimerGetTickCount();
        period--;
        if(period == 0)
        {
            os_printf("Initi tick: { %x } New tick:  { %x }\n", intialTimerTickCount, currentTickCout);
            period = 100;
            __debugbreak();
        }
        // os_printf("Current tick count:{ %x } \n", currentTickCout);
        if((currentTickCout - intialTimerTickCount) > Milliseconds)
        {
            break;
        }
    }

    dumpTickCount = false;
}