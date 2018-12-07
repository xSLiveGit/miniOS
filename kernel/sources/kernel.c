#include "kernel.h"
#include "osrt.h"
#include "os_string.h"
#include "asm_def.h"
#include "interupts.h"
#include "os_timer.h"
#include "pio_mode_ata.h"


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
    
    uint8_t head = 1;
    uint8_t sector = 18;
    uint16_t cylinder = 80;
    uint8_t outBuffer[512] = "ana are mere. Acesta este un string. Am reusit in sfarsit sa scriu ceva ";
    uint8_t inBuffer[512] = { 0 };

    os_printf("I wil write \"Ana are mere on DISK\"\n");
    __debugbreak();
    DskWrite(head, cylinder, sector, outBuffer);
    __debugbreak();

    os_printf("I will read from disk n");
    __debugbreak();
    DskRead(head,cylinder, sector, inBuffer);
    __debugbreak();

    os_printf("I will print read content");
    for(int i=0; i< sizeof("ana are mere. Acesta este un string. Am reusit in sfarsit sa scriu ceva "); i++)
    {
        os_printf("%c", (char)inBuffer[i]);
    }
    os_printf("%\n");

    os_printf("I will wait for 3 secounds\n");
    TimerSleep(3000);
    os_printf("Timeout :)\n");

    __debugbreak();
    
    os_printf("I will hlt :D \n");
    CslInit(&(gEnviroment.Console));
    __hlt();

    return 0;
}