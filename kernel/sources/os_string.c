#include "os_string.h"

int os_strlen(char* Buffer)
{
    char *s = NULL;
    
    for (s = Buffer; *s; ++s);
    
    return (s - Buffer);
}

int os_strcmp(char* Str1, char* Str2)
{
    int len1 = os_strlen(Str1);
    int len2 = os_strlen(Str2);

    if(len1 < len2) return -1;
    if(len1 > len2) return 1;

    for(int i=0; i < len1; i++)
    {
        if(Str1[i] != Str2[i])
        {
            return Str1[i] - Str2[i];
        }
    }

    return 0;
}
