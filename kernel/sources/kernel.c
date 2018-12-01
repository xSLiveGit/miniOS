#include "kernel.h"
#include "osrt.h"
#include "os_string.h"
#include "asm_def.h"
#include "interupts.h"


int main(void)
{
    int a = 1;
    a = a + 1;

    if(!InitEnviroment())
    {
        DebugBreak();
        return -1;
    }
    
    os_printf("Acum testez printf: {%s} si afisez 0xF5 in hex:{%x}\n", "*Acesta este stringul custom*", 245);
    
    os_printf("Acesta e un test. Trebuie sa afisez 235 in dec: %d. Acum afisez 0: %d, Acum afisez -23: %d", 235, 0, -23);
    os_printf("I will dump trapframe: \n");
    AsmIntDumpTrapFrame();
    DebugBreak();

    os_printf("I will initialize idt\n");
    IntInitializeIdt(&(gEnviroment.Idt), &(gEnviroment.IdtDescriptr));

    os_printf("I will divide by 0 and i will expect trap frame: \n");
    DebugBreak();
    // int zero = 1;
    // zero--;
    // int c = 3 / zero;
    // if(zero != 0)
    // {
    //     os_printf("%d\n", c);
    // }

    for(int i=0; i<100; i++)
    {
        DebugBreak();
        os_printf("test\n");
    }

    DebugBreak();

    return 0;
}