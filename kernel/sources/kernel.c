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

    char str[] = "ana are mere";
    char tst_nl[] = "a mers nl";
    // char tst_printf_done[] = "trebuia sa fi fost afisat printf";
    
    int stringSize = os_strlen(str);

    for(int i=0; i < stringSize; i++)
    {
        ScrWriteOnScreen(&gEnviroment.ScreenBuffer, str[i]);
    }
    ScrWriteOnScreen(&gEnviroment.ScreenBuffer, '\n');

    stringSize = sizeof("a mers nl") - 1;
    for(int i=0; i < stringSize; i++)
    {
        ScrWriteOnScreen(&gEnviroment.ScreenBuffer, tst_nl[i]);
    }
    ScrWriteOnScreen(&gEnviroment.ScreenBuffer, '\n');
    // os_printf("print plaintext");
    char format[] = "Acum testez printf: {%s} si afisez 245 in hex:{%x}\n";
    char a_cr[] = "ana are multe mere minunate\n";
    char customStr[] = "*Acesta este stringul custom*";
    os_printf(format, customStr, 245);
    DebugBreak();
    // ScrRemoveFirstLine(&gEnviroment.ScreenBuffer);
    DebugBreak();
    os_printf(a_cr);
    PrvScrMoveLine(1,0,&gEnviroment.ScreenBuffer);
    os_printf(a_cr);
    os_printf(a_cr);
    os_printf(a_cr);
    os_printf(a_cr);
    os_printf(a_cr);
    os_printf(a_cr);
    for(int i=0; i<100; i++)
    {
        char c[] = "test\n";
        os_printf(c);
    }

    // os_printf(a_cr);
    // ScrWriteOnScreen(&gEnviroment.ScreenBuffer, 'a');
    // os_printf("Acum testez printf: {%s}, am printa un sting. Pun newline\n, Acum o sa afisez 245: {%x} in hexa\n", "*Acesta este stringul custom*", 245);

    // DebugBreak();
    // char tst_validation[] = "trebuia sa fi fost afisat printf\n";
    // os_printf(tst_validation);
    // PrvPrintString(tst_validation);
    // stringSize = os_strlen(tst_printf_done);
    
    // for(int i=0; i < stringSize; i++)
    // {
    //     ScrWriteOnScreen(&gEnviroment.ScreenBuffer, tst_printf_done[i]);
    // }
    // ScrWriteOnScreen(&gEnviroment.ScreenBuffer, '\n');

    DebugBreak();

    return 0;
}