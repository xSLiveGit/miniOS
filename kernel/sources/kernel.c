#include "kernel.h"
#include "osrt.h"
#include "os_string.h"

extern 

int main(void)
{
    int a = 1;
    a = a + 1;

    if(!InitEnviroment())
    {
        DebugBreak();
        return -1;
    }

    ClearScreen();

    SCREEN_ITEM* video = (PSCREEN_ITEM) 0xB8000;
    SCREEN_ITEM item = {0};
    item.Color.BackgroundColour = BACKGROUND_COLOUR;
    item.Color.ForegoundColour = COLOUR_GREEN;
    item.Character = 'a';
    *video = item;
    DebugBreak();
    // WriteOnScreen(&gEnviroment.ScreenBuffer, 'a');
    // WriteOnScreen(&gEnviroment.ScreenBuffer, 'n');
    // WriteOnScreen(&gEnviroment.ScreenBuffer, 'a');

    char str[] = "ana are mere";
    char tst_nl[] = "a mers nl";
    
    int stringSize = os_strlen(str);//  sizeof("ana are mere") - 1;

    DebugBreak();
    for(int i=0; i < stringSize; i++)
    {
        WriteOnScreen(&gEnviroment.ScreenBuffer, str[i]);
        DebugBreak();
    }
    WriteOnScreen(&gEnviroment.ScreenBuffer, '\n');

    DebugBreak();
    stringSize = sizeof("a mers nl") - 1;
    for(int i=0; i < stringSize; i++)
    {
        WriteOnScreen(&gEnviroment.ScreenBuffer, tst_nl[i]);
    }
    WriteOnScreen(&gEnviroment.ScreenBuffer, '\n');

    DebugBreak();
    for(int i=0; i<10; i++)
        a++;
    a += 2;
    a += 2;
    a += 2;

    DebugBreak();

    return 0;
}