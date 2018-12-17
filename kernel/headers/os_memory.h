#ifndef _OS_MEMORY_H_
#define _OS_MEMORY_H_

#include "types.h"

#define PDT         0x103000 //there are fill 1st and 2nd entry
#define HEAP_PT     0x106000 //because 105000h & 104000h are reserved

#define HEAP_BASE_ADDRESS       0x1000000
#define PAGE_SIZE               4096
#define HEAP_MAX_PAGES          64
#define TABLE_ENTRIES_NUMBER    512
#define HEAP_IDX_IN_PDT         8

void MmInit(void);
void MmUninit(void);
uint16_t MmReservePage(void);
void MmFreeReservedPage(uint8_t PageIdx);

void* MmAllocPage(void);
void MmFreePage(void*);

#endif //!_OS_MEMORY_H_
