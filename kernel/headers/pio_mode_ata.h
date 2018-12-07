#ifndef _PIO_MODE_ATA_H_
#define _PIO_MODE_ATA_H_

#include "types.h"

#define DSK_STATUS uint64_t 
#define DSK_STATUS_SUCCESS 0

#define DRIVE_HEAD_REGISTER 0x1F6

void DskExecuteStageOne(
    uint8_t     Head, 
    uint16_t    Cylinder, 
    uint8_t     Sector
);

DSK_STATUS DskRead(
    uint8_t     Head, 
    uint16_t    Cylinder, 
    uint8_t     Sector, 
    uint8_t*    BufferOf512
);

DSK_STATUS DskWrite(
    uint8_t     Head, 
    uint16_t    Cylinder, 
    uint8_t     Sector, 
    uint8_t*    BufferOf512
);



#endif // !_PIO_MODE_ATA_H_