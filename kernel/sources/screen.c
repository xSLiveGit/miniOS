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
    ClearScreen(BACKGROUND_COLOUR);
    uint8_t screenOffset = ScrGetOffset(ScreenBuffer->Line, ScreenBuffer->Columns);

    for(i=0; i < screenOffset; i++)
    {
        *video = ScreenBuffer->Buffer[i];
        video++;
    }
}

uint8_t ScrGetOffset(uint8_t line, uint8_t column)
{
    return line * MAX_COLUMNS + column;
}

bool WriteInBuffer(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
)
{
    DebugBreak();

    SCREEN_ITEM item = {0};
    int maxNumber = 0;

    maxNumber = ScreenBuffer->Columns > ScreenBuffer->IdxInColumn + 1
        ? ScreenBuffer->Columns
        : ScreenBuffer->IdxInColumn + 1;

    if (maxNumber >= MAX_COLUMNS)
        return false; //here i can delete 1st line

    if(Char == '\n')
    {
        if(ScreenBuffer->Line + 1 >= MAX_LINES)
            return false;

        ScreenBuffer->Line++;
        ScreenBuffer->IdxInColumn = 0;
        ScreenBuffer->Columns = 0;
        return true;
    }

    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = COLOUR_GREEN;
    item.Character = Char;

    DebugBreak();
    uint8_t screenOffset = ScrGetOffset(ScreenBuffer->Line, ScreenBuffer->IdxInColumn);
    ScreenBuffer->Buffer[screenOffset] = item;
    ScreenBuffer->IdxInColumn+=1;
    // ScreenBuffer->NoItemsUse+=1;//temp to test

    if(ScreenBuffer->IdxInColumn >= ScreenBuffer->Columns)
        ScreenBuffer->Columns++;

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