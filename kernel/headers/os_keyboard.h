#ifndef _OS_KEYBOARD_H_
#define _OS_KEYBOARD_H_

#include "types.h"

#pragma pack(push, 1)
typedef struct _KEYBOARD_INFO
{
    bool IsCapsLockOn   : 1;
    bool IsNumLockOn    : 1;
} KEYBOARD_INFO, *PKEYBOARD_INFO;
#pragma pack(pop)

void IsrKeyboardKeyHandler(uint8_t);

#endif // ! _OS_KEYBOARD_H_