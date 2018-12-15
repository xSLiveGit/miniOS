#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "screen.h"
#include "interupts.h"
#include "os_keyboard.h"
#include "os_console.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)

#pragma pack(push, 1)
typedef struct _GLOBALS
{
    SCREEN_BUFFER   ScreenBuffer;
    IDT             Idt;
    IDT_INFO        IdtDescriptr;
    KEYBOARD_INFO   KeyboardInfo;
    bool            AcceptCmds;
    CONSOLE         Console;             
} GLOBALS, *PGLOBALS;
#pragma pack(pop)

#endif // !_GLOBALS_H_