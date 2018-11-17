#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "types.h"

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define MAX_OFFSET      25 * 80

typedef enum _COLOUR
{
    COLOUR_BLACK = 0x00
    , COLOUR_BLUE = 0x01
    , COLOUR_GREEN = 0x02
    , COLOUR_CYAN = 0x03
    , COLOUR_RED = 0x04
    , COLOUR_MAGENTA = 0x05
    , COLOUR_BROWN = 0x06
    , COLOUR_LIGHT_GREY = 0x07
    , COLOUR_DARK_GREY = 0x08
    , COLOUR_LIGHT_BLUE = 0x09
    , COLOUR_LIGHT_GREEN = 0x0A
    , COLOUR_LIGHT_CYAN = 0x0B
    , COLOUR_LIGHT_RED = 0x0C
    , COLOUR_LIGHT_MAGENTA = 0x0D
    , COLOUR_LIGHT_BROWN = 0X0E
    , COLOUR_WHITE = 0X0F
} COLOUR, *PCOLOUR;

#pragma pack(push, 1)
typedef struct _COLOR_ITEM
{
    union
    {
        struct
        {
            uint8_t ForegroudColour             : 3;
            uint8_t ForegroudColourBrightBit    : 1;
        } SegmentedColor;
        uint8_t EntireColor : 4; 
    } ForegoundColour;

    union
    {
        struct
        {
            uint8_t ForegroudColour             : 3;
            uint8_t ForegroudColourBrightBit    : 1;
        } SegmentedColor;
        uint8_t EntireColor : 4; 
    } BackgroundColour;

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
    PSCREEN_ITEM    Buffer[MAX_OFFSET];
    uint16_t        NoBytesUser;
    uint16_t        CurrentIdx;
} SCREEN_BUFFER, *PSCREEN_BUFFER;
#pragma pack(pop)



void FlushScreenBufferOnScreen(
    PSCREEN_BUFFER ScreenBuffer
);

void WriteInBuffer(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
);

void WriteOnScreen(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
);

#endif //_SCREEN_H_