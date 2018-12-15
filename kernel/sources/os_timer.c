#include "os_timer.h"
#include "os_definitions.h"
#include "asm_def.h"
#include "types.h"
#include "osrt.h"

static volatile uint32_t gTickCount;

void TimerInit(void)
{
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

    __interlock_increment_uint32t(&gTickCount);
    __outb(PIC_MASTER_CTRL, PIC_EOI);

    __sti();
}

uint32_t TimerGetTickCount(void)
{
    return gTickCount;
}

void TimerSleep(uint32_t Milliseconds)
{
    os_printf("TimerSleep milisec: {%x} \n", Milliseconds);
    volatile uint32_t intialTimerTickCount = 0;
    volatile uint32_t currentTickCout = 0; 
    volatile uint32_t dif = 0;
    
    intialTimerTickCount = TimerGetTickCount();
    currentTickCout = TimerGetTickCount();
    dif = (currentTickCout - intialTimerTickCount);

    while(true)
    {
        currentTickCout = TimerGetTickCount();
        dif = (currentTickCout - intialTimerTickCount); 
        if(dif > Milliseconds)
        {
            os_printf(
                "init tck: {%x}, current tck: {%x} dif: {%x}\n", 
                intialTimerTickCount, 
                currentTickCout,
                dif
            );
            break;
        }
    }
}