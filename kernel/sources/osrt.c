#include "osrt.h"
#include "os_string.h"

//trus means success
bool InitEnviroment()
{
    os_memset(&gEnviroment, sizeof(gEnviroment), 0);
    ScrClearScreen();
    return true;
}

void os_memset(
    void*       Destionation
    , size_t    Size
    , uint8_t   Byte
)
{
    uint8_t* start = (uint8_t*)(Destionation);
    uint8_t* end = start + Size;
    while(start != end)
    {
        *start++ = Byte;
    }
}

char PrvHexdigitToChar(
    uint8_t Hexdigit
)
{
    return (Hexdigit < 10) ?  ('0' + Hexdigit) :  'A' + (Hexdigit - 10);  
}

void PrvPrintString(char* Buffer)
{
    uint16_t bufferLen = os_strlen(Buffer);
    if(bufferLen == 0)
        return;

    for(int i=0; i < bufferLen-1; i++)//prepare buffer
        ScrWriteInBuffer(&gEnviroment.ScreenBuffer, Buffer[i]);
    ScrWriteOnScreen(&gEnviroment.ScreenBuffer, Buffer[bufferLen - 1]);
}

void PrvPrinfHex(uint64_t Number)
{
    char result[17] = {0};
    uint16_t size = 16;

    for (uint16_t i = 0; i < size; ++i)
    {
        result[i] = '0';
    }

    while (Number != 0)
    {
        Number = Number / 16;
        result[--size] = PrvHexdigitToChar(Number % 16); 
    }

    PrvPrintString(result);
}

void PrvPrintChar(char Character)
{
    ScrWriteOnScreen(&gEnviroment.ScreenBuffer, Character);
}

void os_printf(
    char*   Format
    , ...
)
{
    DebugBreak();
    void* argument = (void*)(&Format);

    while(*Format != 0)
    {
        char currentChar = *Format;
        Format++;
        if (currentChar != '%')
        {
            PrvPrintChar(currentChar);
            continue;
        }

        char type = *Format++;
        argument = (char*)(argument) + sizeof(void*);

        switch (type)
        {
        case 'x':
            PrvPrinfHex(*(uint64_t*)argument);
            break;
        case 's':
            PrvPrintString(*(char**)argument);
            break;
        }
    }
}
