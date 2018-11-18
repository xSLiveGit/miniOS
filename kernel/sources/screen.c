#include "screen.h"
#include "asm_def.h"

void ClearScreen()
{
    PSCREEN_ITEM video = ( SCREEN_ITEM*)0xB8000;
    SCREEN_ITEM item = {0};

    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = BACKGROUND_COLOUR;
    item.Character = ' ';

    for(int i=0; i<MAX_OFFSET; i++)
    {
        *video= item;
        video++;
    }
}

void FlushScreenBufferOnScreen(
    PSCREEN_BUFFER ScreenBuffer
)
{
    int i = 0;
    // volatile SCREEN_ITEM* video = (volatile char*)0xB8000;
    SCREEN_ITEM* video = (PSCREEN_ITEM) 0xB8000;
    // ClearScreen(BACKGROUND_COLOUR);

    for(i=0; i < ScreenBuffer->NoItemsUse; i++)
    {
        *video = ScreenBuffer->Buffer[i];
        video++;
    }
}

bool WriteInBuffer(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
)
{
    DebugBreak();

    SCREEN_ITEM item = {0};
    // int maxNumber = 0;

    // maxNumber = ScreenBuffer->NoItemsUse > ScreenBuffer->CurrentIdx + 1
    //     ? ScreenBuffer->NoItemsUse
    //     : ScreenBuffer->CurrentIdx + 1;

    // if (maxNumber >= MAX_OFFSET)
    //     return false; //here i can delete 1st line

    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = COLOUR_GREEN;
    item.Character = Char;

    DebugBreak();
    ScreenBuffer->Buffer[ScreenBuffer->CurrentIdx] = item;
    ScreenBuffer->CurrentIdx+=1;
    ScreenBuffer->NoItemsUse+=1;//temp to test

    // if(ScreenBuffer->CurrentIdx >= ScreenBuffer->NoItemsUse)
    //     ScreenBuffer->NoItemsUse++;

    return true;
}

void WriteOnScreen(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
)
{
    // ClearScreen();
    WriteInBuffer(ScreenBuffer, Char);
    FlushScreenBufferOnScreen(ScreenBuffer);
}