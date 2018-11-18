#include "osrt.h"

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

//trus means success
bool InitEnviroment()
{
    os_memset(&gEnviroment, sizeof(gEnviroment), 0);
    ClearScreen();
    return true;
}
