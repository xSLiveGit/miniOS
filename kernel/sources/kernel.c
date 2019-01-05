#include "kernel.h"
#include "osrt.h"
#include "os_string.h"
#include "asm_def.h"
#include "interupts.h"
#include "os_timer.h"
#include "pio_mode_ata.h"


int main(void)
{
    if(!InitEnviroment())
    {
        __debugbreak();
        return -1;
    }
    
    CslInit(&(gEnviroment.Console));
    while(true)
    {
        while(true) 
        {
            if(gEnviroment.AcceptCmds == false)
                break;
        }
        
        CslHandleCmd(&(gEnviroment.Console));
    }

    while(true)
    {
        __hlt();
    }

    return 0;
}