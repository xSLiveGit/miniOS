#include "pio_mode_ata.h"
#include "asm_def.h"

#define SECTOR_SIZE 512

#define DSK_REGISTER_DATA 0x1F0
#define DSK_REGISTER_SECTOR_COUNT 0x1F2
#define DSK_REGISTER_SECTOR_NUMBER 0x1F3
#define DSK_REGISTER_CYLINDER_LOW 0x1F4
#define DSK_REGISTER_CYLINDER_HIGH 0x1F5
#define DSK_REGISTER_DRIVER_HEADER 0x1F6
#define DSK_REGISTER_COMMAND 0x1F7

#define DSK_COMMAND_READ 0x20
#define DSK_COMMAND_WRITE 0x30

void 
DskExecuteCommandPrologue(
    uint8_t     Head, 
    uint16_t    Cylinder, 
    uint8_t     Sector,
    uint8_t     Command
)
{
    //Sector Count
    __outb(DSK_REGISTER_DRIVER_HEADER, (Head | 0b10100000)); //Master DRV | CHS mode 
    __outb(DSK_REGISTER_SECTOR_COUNT, 1); // 1 sector
    __outb(DSK_REGISTER_SECTOR_NUMBER, Sector);
    __outb(DSK_REGISTER_CYLINDER_LOW, ((uint8_t*)&Cylinder)[0]);
    __outb(DSK_REGISTER_CYLINDER_HIGH, ((uint8_t*)&Cylinder)[1]);
    __outb(DSK_REGISTER_COMMAND, Command);
    while(true)
    {
        uint8_t retVal = __inb(DSK_REGISTER_COMMAND);//wait for DRQ
        if(retVal & 8)
            break;
    }
}

DSK_STATUS 
DskRead(
    uint8_t     Head
    , uint16_t  Cylinder 
    , uint8_t   Sector
    , uint8_t*  BufferOf512
)
{
    DskExecuteCommandPrologue(Head, Cylinder, Sector, DSK_COMMAND_READ);
    __dsk_read_sector(BufferOf512);
    return DSK_STATUS_SUCCESS;
}

DSK_STATUS 
DskWrite(
    uint8_t     Head 
    , uint16_t  Cylinder 
    , uint8_t   Sector
    , uint8_t*  BufferOf512
)
{
    // DiskWriteSector(Cylinder, Sector, Head, BufferOf512);
    // __debugbreak();
    // DiskIOSector(Cylinder, Sector, Head, BufferOf512, DSK_COMMAND_WRITE);
    // DiskWriteBuffer(Cylinder, Sector, Head, BufferOf512);

    // __debugbreak();
    DskExecuteCommandPrologue(Head, Cylinder, Sector, DSK_COMMAND_WRITE);
    __dsk_write_sector(BufferOf512);

    // os_printf("Cmd\n");
    // __debugbreak();
    // __outb(DSK_REGISTER_COMMAND, DSK_COMMAND_WRITE);
    // while(true)
    // {
    //     uint8_t retVal = __inb(DSK_REGISTER_COMMAND);
    //     if(retVal & 8)
    //         break;
    // }
    

    // // __debugbreak();
    // DiskWriteBuffer(Cylinder, Sector, Head, BufferOf512);
    // os_printf("A recut de DiskWriteBuffer\n");
    // // os_printf("A trecut de IO Sector\n");
    // // __dsk_write_byte_string(SECTOR_SIZE, DSK_REGISTER_DATA, BufferOf512);//asta pare ok

    return DSK_STATUS_SUCCESS;
}

