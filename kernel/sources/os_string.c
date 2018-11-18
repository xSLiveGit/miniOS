#include "os_string.h"

int os_strlen(char* Buffer)
{
    char *s = NULL;
    
    for (s = Buffer; *s; ++s);
    
    return (s - Buffer);
}