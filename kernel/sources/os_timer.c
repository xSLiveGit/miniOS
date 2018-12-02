#include "os_timer.h"
#include "os_definitions.h"
#include "asm_def.h"
#include "types.h"

uint64_t gTickCount;

void TimerInit(void)
{
    uint32_t desiredFreq = 1000;
    uint32_t timerBaseFreq = 1193180;  //Magic value from https://wiki.osdev.org/Programmable_Interval_Timer :) 
    uint16_t divisor = timerBaseFreq / desiredFreq;

    uint8_t lowByte = (&divisor)[0];
    uint8_t highByte = (&divisor)[1];

    __outb(TIMER_COMMAND_PORT, 0x36); //SQUARE-WAVE mode | 16 bit counter | lobyte and hibyte
    __outb(TIMER_CHANNEL0_PORT, lowByte);
    __outb(TIMER_CHANNEL0_PORT, highByte);
}

void IsrTimer(void)
{
    __cli();

    __interlock_increment_uint64t(&gTickCount);
    __outb(PIC_MASTER_CTRL, PIC_EOI);

    __sti();
}

uint64_t TimerGetTickCount(void)
{
    return gTickCount;
}

void TimerSleep(uint64_t Milliseconds)
{
    uint64_t intialTimerTickCount = TimerGetTickCount();
    while(TimerGetTickCount() - intialTimerTickCount < Milliseconds); //best Sleep ever...
}