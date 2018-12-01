#include "screen.h"
#include "asm_def.h"

void ScrClearScreen()
{
    PSCREEN_ITEM video = (SCREEN_ITEM *)0xB8000;
    SCREEN_ITEM item = {0};

    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = BACKGROUND_COLOUR;
    item.Character = ' ';

    for (int i = 0; i < MAX_OFFSET; i++)
    {
        *video = item;
        video++;
    }
}

void ScrFlushScreenBufferOnScreen(
    PSCREEN_BUFFER ScreenBuffer)
{
    int i = 0;
    SCREEN_ITEM *video = (PSCREEN_ITEM)0xB8000;

    if (ScreenBuffer->Columns == 0)
        return;

    ScrClearScreen(BACKGROUND_COLOUR);

    uint16_t offset = ScrGetOffset(ScreenBuffer->Line, ScreenBuffer->Columns);
    for (i = 0; i < offset; i++)
    {
        *video = ScreenBuffer->Buffer[i];
        video++;
    }
}

uint16_t ScrGetOffset(
    uint16_t Line, 
    uint16_t Column
    )
{
    return Line * MAX_COLUMNS + Column;
}

bool ScrWriteInBuffer(
    PSCREEN_BUFFER  ScreenBuffer,
    char            Char
)
{
    SCREEN_ITEM item = {0};
    int maxNumber = 0;

    maxNumber = ScreenBuffer->Columns > ScreenBuffer->IdxInColumn + 1
                    ? ScreenBuffer->Columns
                    : ScreenBuffer->IdxInColumn + 1;

    if (maxNumber >= MAX_COLUMNS && Char != '\n') //generate force nl
    {
        ScrWriteInBuffer(ScreenBuffer, '\n');
    }

    if (ScreenBuffer->Line >= MAX_LINES)
    {
        ScrRemoveFirstLine(ScreenBuffer);
    }

    if (Char == '\n')
    {
        if (ScreenBuffer->Line + 1 >= MAX_LINES)
            ScrRemoveFirstLine(ScreenBuffer);

        ScreenBuffer->Line++;
        ScreenBuffer->IdxInColumn = 0;
        ScreenBuffer->Columns = 0;
        return true;
    }

    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = COLOUR_GREEN;
    item.Character = Char;

    uint16_t screenOffset = ScrGetOffset(ScreenBuffer->Line, ScreenBuffer->IdxInColumn);
    ScreenBuffer->Buffer[screenOffset] = item;
    ScreenBuffer->IdxInColumn += 1;

    if (ScreenBuffer->IdxInColumn >= ScreenBuffer->Columns)
        ScreenBuffer->Columns++;

    return true;
}

void ScrWriteOnScreen(
    PSCREEN_BUFFER  ScreenBuffer
    , char          Char
)
{
    ScrWriteInBuffer(ScreenBuffer, Char);
    ScrFlushScreenBufferOnScreen(ScreenBuffer);
}


void PrvScrMoveLine(
    uint16_t        SourceLine,
    uint16_t        DestionationLine,
    PSCREEN_BUFFER  ScreenBuffer
)
{
    if(SourceLine == DestionationLine)
        return;

    if(SourceLine >= MAX_LINES || DestionationLine >= MAX_LINES)
        return;

    PSCREEN_ITEM source = &(ScreenBuffer->Buffer[SourceLine]);
    PSCREEN_ITEM destionation = &(ScreenBuffer->Buffer[DestionationLine]);
    for(int i=0 ;i<MAX_COLUMNS; i++)
    {
        *destionation = *source;
        destionation++;
        source++;
    }
}

void ScrRemoveFirstLine(PSCREEN_BUFFER ScreenBuffer)
{
    // uint16_t totalNumbersOfMovedItems = ScrGetOffset(ScreenBuffer->Line, ScreenBuffer->Columns);
    for (int i = 0; i < MAX_COLUMNS * MAX_LINES - MAX_COLUMNS; i++)
    {
        ScreenBuffer->Buffer[i] = ScreenBuffer->Buffer[i + MAX_COLUMNS];
    }

    ScreenBuffer->Line--;
    ScrFlushScreenBufferOnScreen(ScreenBuffer);
}


void ScrRemoveCharFromCurrentLine(
    PSCREEN_BUFFER ScreenBuffer
)
{
    if(ScreenBuffer->Columns == 0)
    { 
        return;
    }


    ScreenBuffer->Columns--;
    ScreenBuffer->IdxInColumn--;
    ScrWriteInBuffer(ScreenBuffer, ' ');
    ScrFlushScreenBufferOnScreen(ScreenBuffer);
    ScreenBuffer->Columns--;
    ScreenBuffer->IdxInColumn--;
}