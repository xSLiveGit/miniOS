#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_

#include "types.h"
#define TIMER_COMMAND_PORT      0x43
// CHANNEL 0 port is bounded to IRQ0
#define TIMER_CHANNEL0_PORT     0x40   

void IsrTimer(void);
void TimerInit(void);

uint64_t TimerGetTickCount(void);
void TimerSleep(uint64_t);

#endif // _OS_TIMER_H_