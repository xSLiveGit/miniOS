#include "kernel.h"

int main(void)
{
    DebugBreak();
    int a = 1;
    a = a + 1;
    DebugBreak();
    
    for(int i=0; i<10; i++)
        a++;
    a += 2;
    a += 2;
    a += 2;

    DebugBreak();

    return 0;
}