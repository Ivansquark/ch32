/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* Declarations of disk functions */
#include "ff.h"     /* Obtains integer types */
#include "sd.h"
#include "w25q.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
/* Physical drive nmuber to identify the drive */
DSTATUS disk_status([[maybe_unused]] BYTE pdrv) {
    // DSTATUS stat;
    // int result;
    // switch (pdrv) {
    // case DEV_RAM :
    //	//result = RAM_disk_status(); // translate the reslut code here
    //	return stat;
    // case DEV_MMC :
    //	//result = MMC_disk_status(); // translate the reslut code here
    //	return stat;
    // case DEV_USB :
    //	//result = USB_disk_status(); // translate the reslut code here
    //	return stat;
    //}
    // return STA_NOINIT;
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
/* Physical drive nmuber to identify the drive */
DSTATUS disk_initialize([[maybe_unused]] BYTE pdrv) {
    // DSTATUS stat;
    // int result;
    // switch (pdrv) {
    // case DEV_RAM :	//result = RAM_disk_initialize(); // translate the
    // reslut code here 	return stat; case DEV_MMC :	//result =
    // MMC_disk_initialize(); // translate the reslut
    // code here 	return stat; case DEV_USB : //result =
    // USB_disk_initialize();// translate the reslut code here 	return stat;
    //}
    // return STA_NOINIT;
    return 0; // already initialized
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read([[maybe_unused]] BYTE
                      pdrv,   /* Physical drive nmuber to identify the drive */
                  BYTE* buff, /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
    [[maybe_unused]] DRESULT res;
    [[maybe_unused]] int result;
    // result =
    // SD::pThis->ReadBlock(sector,(uint32_t*)buff,count*512);//MMC_disk_read(buff,
    // sector, count);
    // result = Sd::pThis->SD_ReadDisk(buff, sector, count * 512);
    W25q::pThis->read(buff, 4096 + sector, count * 512);
    return RES_OK;
    // switch (pdrv) {
    // case DEV_RAM : // translate the arguments here //result =
    // RAM_disk_read(buff, sector, count); // translate the reslut code here
    //	return res;
    // case DEV_MMC :
    //	result =
    // SD::pThis->ReadBlock(sector,(uint32_t*)buff,count);//MMC_disk_read(buff,
    // sector, count); 	return res;
    //
    // case DEV_USB :
    //	//result = USB_disk_read(buff, sector, count);
    //
    //	return res;
    //}
    // return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write([[maybe_unused]] BYTE
                       pdrv, /* Physical drive nmuber to identify the drive */
                   const BYTE* buff, /* Data to be written */
                   LBA_t sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
    [[maybe_unused]] DRESULT res;
    [[maybe_unused]] int result;
    // result = SD::pThis->WriteBlock(sector, (uint32_t*)buff, count * 512);
    // result = Sd::pThis->SD_WriteDisk((uint8_t*)buff, sector, count * 512);
    // 255 page size  4096 page erase size
    W25q::pThis->write((uint8_t*)buff, 4096 + sector, count * 512);
    return RES_OK;
    // switch (pdrv) {
    // case DEV_RAM :	//result = RAM_disk_write(buff, sector, count);
    //	return res;
    //
    // case DEV_MMC :
    //	//result =
    // SD::pThis->WriteBlock(sector,(uint32_t*)buff,count);//MMC_disk_write(buff,
    // sector, count); 	return res;
    //
    // case DEV_USB : 	//result = USB_disk_write(buff, sector, count);
    //	return res;
    //}
    // return RES_PARERR;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT
disk_ioctl([[maybe_unused]] BYTE pdrv, /* Physical drive nmuber (0..) */
           BYTE cmd,                   /* Control code */
           [[maybe_unused]] void* buff /* Buffer to send/receive control data */
) {
    [[maybe_unused]] DRESULT res;
    [[maybe_unused]] int result;
    if (cmd == CTRL_SYNC) {
        return RES_OK;
    } else if (GET_SECTOR_SIZE) {
        *(DWORD*)buff = 512;
        return RES_OK;
    } else if (GET_SECTOR_COUNT) {
        *(DWORD*)buff = 128;
        return RES_OK;
    } else if (GET_BLOCK_SIZE) {
        *(UINT*)buff = 1;
        return RES_OK;
    } else {
        return RES_OK;
        // return RES_ERROR;
    }
    // switch (pdrv) {
    // case DEV_RAM : // Process of the command for the RAM drive
    //	return res;
    // case DEV_MMC :
    //	// Process of the command for the MMC/SD card
    //	return res;
    // case DEV_USB :	// Process of the command the USB drive
    //	return res;
    //}
    // return RES_PARERR;
}
