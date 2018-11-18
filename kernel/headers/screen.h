#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "types.h"

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define MAX_OFFSET      25 * 80

#define COLOUR_BLACK 0x00
#define COLOUR_BLUE  0x01
#define COLOUR_GREEN  0x02
#define COLOUR_CYAN  0x03
#define COLOUR_RED  0x04
#define COLOUR_MAGENTA  0x05
#define COLOUR_BROWN  0x06
#define COLOUR_LIGHT_GREY  0x07
#define COLOUR_DARK_GREY  0x08
#define COLOUR_LIGHT_BLUE  0x09
#define COLOUR_LIGHT_GREEN  0x0A
#define COLOUR_LIGHT_CYAN  0x0B
#define COLOUR_LIGHT_RED  0x0C
#define COLOUR_LIGHT_MAGENTA  0x0D
#define COLOUR_LIGHT_BROWN  0X0E
#define COLOUR_WHITE  0X0F


#define BACKGROUND_COLOUR COLOUR_BROWN

#pragma pack(push, 1)
typedef struct _COLOR_ITEM
{
    uint8_t ForegoundColour : 4;
    uint8_t BackgroundColour : 4;
} COLOR_ITEM, *PCOLOR_ITEM;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _SCREEN_ITEM
{
    char        Character;
    COLOR_ITEM  Color;
}SCREEN_ITEM, *PSCREEN_ITEM;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _SCREEN_BUFFER
{
    SCREEN_ITEM     Buffer[MAX_OFFSET];
    uint16_t        NoItemsUse;
    uint16_t        CurrentIdx;
} SCREEN_BUFFER, *PSCREEN_BUFFER;
#pragma pack(pop)

void FlushScreenBufferOnScreen(
    PSCREEN_BUFFER ScreenBuffer
);

void ClearScreen();

//reuturn true if write smth
bool WriteInBuffer(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
);

void WriteOnScreen(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
);

#endif //_SCREEN_H_