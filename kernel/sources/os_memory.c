#include "os_memory.h"
#include "osrt.h"

uint64_t g_MemoryAvailable;

#define MM_HEAP_MM_FOR_IDX(PageIdx) ((void*)((uint8_t*)HEAP_BASE_ADDRESS + PAGE_SIZE * PageIdx))

void MmInit(void)
{
    g_MemoryAvailable = 0;
    //PDT[1] = HEAP_PT
    //  0 - user mode / 1 - R-W / 1 - PT present
    *((uint32_t*)(((uint8_t*)PDT) + 8 * HEAP_IDX_IN_PDT)) = HEAP_PT | 3; 
    uint32_t** heapPt = (uint32_t**)HEAP_PT;

    //  0 - user mode / 1 - R-W / 0 - page not present
    uint32_t* currentPage = (uint32_t*)((uint64_t)HEAP_BASE_ADDRESS | 0b010); 

    for(size_t i=0; i < TABLE_ENTRIES_NUMBER; i++)
    {
        heapPt[i] = currentPage;
        currentPage += PAGE_SIZE;
    }
}

void MmUninit(void)
{
    for(uint8_t i=0; i<HEAP_MAX_PAGES; i++)
    {
        if(g_MemoryAvailable & (1 << i))//if memory is allocated
        {
            MmFreeReservedPage(i);
        }
    }

    *((uint32_t*)(((uint8_t*)PDT) + 8)) = 0;//free 2nd entry from PDT 
}

uint8_t MmReservePage(void)
{
    bool found = false;

    if(g_MemoryAvailable + 1 == 0)//no available pages
        return UINT8_MAX;

    for(int i=0; ((i < HEAP_MAX_PAGES) && (!found)); i++)
    {
        if((g_MemoryAvailable & (1 << i)) == 0)
        {
            found = true;
            g_MemoryAvailable = (g_MemoryAvailable | (1 << i));
            return i;
        }
    }
    return UINT8_MAX;
}


void MmFreeReservedPage(uint8_t PageIdx)
{
    uint32_t* heapPt = (uint32_t*)HEAP_PT;

    if((g_MemoryAvailable & (1 << PageIdx)) == 0)//page is not reserved
    {
        return;
    }

    heapPt[PageIdx] &= (~1);//mark as not present
    void* addrToFree = MM_HEAP_MM_FOR_IDX(PageIdx);
    __invlpg(addrToFree);
}

void* MmAllocPage(void)
{
    uint8_t reservedPageIdx = MmReservePage();
    if(reservedPageIdx == UINT8_MAX)
    {
        return NULL;
    }

    return MM_HEAP_MM_FOR_IDX(reservedPageIdx);
}

void MmFreePage(void* Addr)
{
    if((uint64_t)Addr % PAGE_SIZE)//given addr is not multiple of 64
        return;

    if((uint64_t)Addr < (uint64_t)HEAP_BASE_ADDRESS)
        return;

    uint64_t idx = ((uint8_t*)Addr - (uint8_t*)HEAP_BASE_ADDRESS) / PAGE_SIZE;

    if(idx > HEAP_MAX_PAGES)//bad addr
        return;

    MmFreeReservedPage(idx);
}