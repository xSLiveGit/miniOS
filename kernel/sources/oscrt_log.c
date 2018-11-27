#include "oscrt_log.h"

void LogCritical()
{
    char crt_msg[] = "CRITICAL MESSAGE\n";
    os_printf(crt_msg);
}