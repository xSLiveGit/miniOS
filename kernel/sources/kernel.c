#include "kernel.h"
#include "osrt.h"
#include "os_string.h"
#include "asm_def.h"
#include "interupts.h"
#include "os_timer.h"


int main(void)
{
    int a = 1;
    a = a + 1;

    if(!InitEnviroment())
    {
        __debugbreak();
        return -1;
    }
    
    os_printf("Acum testez printf: {%s} si afisez 0xF5 in hex:{%x}\n", "*Acesta este stringul custom*", 245);
    os_printf("Acesta e un test. Trebuie sa afisez 235 in dec: %d. Acum afisez 0: %d, Acum afisez -23: %d and print char i: '%c'\n", 235, 0, -23, 'i');

    os_printf("I will wait for 10 secounds\n");
    __debugbreak();

    TimerSleep(10000);

    os_printf("Timeout :)\n");
    __debugbreak();
    
    os_printf("I will hlt :D \n");
    __hlt();

    return 0;
}