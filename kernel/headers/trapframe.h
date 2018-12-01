#ifndef _TRAP_FRAME_H_
#define TRAP_FRAME_H_

#include "osrt.h"
#include "os_definitions.h"


void IsrCritical(void);

void TrapFrame64Dump(PTRAP_FRAME_64 TrapFrame);

# endif // ! TRAP_FRAME_H_

