#ifndef _OS_KEYBOARD_H_
#define _OS_KEYBOARD_H_

#include "types.h"

#define KYBRD_ENC_INPUT_BUF	    0x60
#define KYBRD_ENC_CMD_REG	    0x60
#define KYBRD_CTRL_STATS_REG    0x64
#define KYBRD_CTRL_CMD_REG      0x64

#ifndef PIC_EOI
#define PIC_EOI		    0x20	
#endif //! PIC_EOI 

void IsrKeyboard(void);

#pragma pack(push, 1)
typedef struct _KEYBOARD_INFO
{
    bool IsCapsLockOn   : 1;
    bool IsNumLockOn    : 1;
} KEYBOARD_INFO, *PKEYBOARD_INFO;
#pragma pack(pop)


void IsrKeyboardKeyHandler(uint8_t);

#endif // ! _OS_KEYBOARD_H_