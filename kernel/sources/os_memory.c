#include "os_memory.h"
#include "osrt.h"
#include "asm_def.h"

uint64_t g_MemoryAvailable;

#define MM_HEAP_MM_FOR_IDX(PageIdx) ((void*)((uint8_t*)HEAP_BASE_ADDRESS + PAGE_SIZE * PageIdx))

void MmInit(void)
{
    g_MemoryAvailable = 0;
    
    //  0 - user mode / 1 - R-W / 1 - PT present
    uint64_t* pdt = (uint64_t*)PDT;
    pdt[8] = (uint64_t)(HEAP_PT | 3);
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

uint16_t MmReservePage(void)
{
    if(g_MemoryAvailable == UINT64_MAX)//no available pages
    {
        return UINT16_MAX;
    }

    for(uint16_t i=0; i < HEAP_MAX_PAGES; i++)
    {
        if((g_MemoryAvailable & ((uint64_t)1 << i)) == 0)
        {
            g_MemoryAvailable = (g_MemoryAvailable | (((uint64_t)1) << i));

            uint64_t** heapPt = (uint64_t**)HEAP_PT;
            heapPt[i] =  (uint64_t*)(((uint64_t)(heapPt[i])) | 1);
            __invlpg(heapPt[i]);

            return i;
        }
    }

    return UINT16_MAX;
}


void MmFreeReservedPage(uint8_t PageIdx)
{
    uint32_t* heapPt = (uint32_t*)HEAP_PT;

    if((g_MemoryAvailable & ((((uint64_t)1) << PageIdx))) == 0)//page is not reserved
    {
        os_printf("[ERR] page is not reserver: %d", PageIdx);
        return;
    }

    heapPt[PageIdx] &= (~1);//mark as not present
    g_MemoryAvailable &= ~(1UL << PageIdx); 
    void* addrToFree = MM_HEAP_MM_FOR_IDX(PageIdx);
    __invlpg(addrToFree);
}

void* MmAllocPage(void)
{
    uint16_t reservedPageIdx = MmReservePage();

    if(reservedPageIdx == UINT16_MAX)
    {
        return NULL;
    }
    
    return MM_HEAP_MM_FOR_IDX(reservedPageIdx);
}

void MmFreePage(void* Addr)
{
    if(((uint64_t)Addr - HEAP_BASE_ADDRESS) % PAGE_SIZE)//given addr is not multiple of 64
    {
        os_printf("[ERR] adresa nu e multiplu de pagina");
        return;
    }

    uint64_t idx = ((uint8_t*)Addr - (uint8_t*)HEAP_BASE_ADDRESS) / PAGE_SIZE;

    if(idx > HEAP_MAX_PAGES)//bad addr
    {
        os_printf("[ERR] idx prea mare: %d", idx);
        return;
    }

    MmFreeReservedPage(idx);
}