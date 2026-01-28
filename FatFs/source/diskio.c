/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2025        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* Declarations FatFs MAI */

#include <stdint.h>
#include <string.h>  // for memcpy

#include "ff.h"                  /* Basic definitions of FatFs */
#include "stm32u3_flash_disk.h"  // for FLASH disk functions
#include "stm32u3_ram_disk.h"    // for RAM disk functions

/* Example: Mapping of physical drive number for each drive */
#define DEV_RAM 0   /* Map Ramdisk to physical drive 0 */
#define DEV_FLASH 1 /* Map FTL to physical drive 1 */
// define DEV_MMC 2   /* Map MMC/SD card to physical drive 2 */
// #define DEV_USB 3   /* Map USB MSD to physical drive 3 */
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
  // DSTATUS stat;
  // int result;

  switch (pdrv) {
    case DEV_RAM:
      return 0;
    case DEV_FLASH:
      return 0;
    default:
      return STA_NOINIT;
  }
  return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
  // DSTATUS stat;
  // int result;

  switch (pdrv) {
    case DEV_RAM:
      RAM_disk_initialize();
      return RES_OK;
    case DEV_FLASH:
      FLASH_disk_initialize();
      return RES_OK;
    default:
      return STA_NOINIT;
  }
  return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,  /* Physical drive nmuber to identify the drive */
                  BYTE *buff, /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
  DRESULT res;
  int result;

  switch (pdrv) {
    case DEV_RAM:
      RAM_disk_read(buff, sector, count);
      return RES_OK;
    case DEV_FLASH:
      FLASH_disk_read(buff, sector, count);
      return RES_OK;
    default:
      return RES_PARERR;
  }
  return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv, /* Physical drive nmuber to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   LBA_t sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
  DRESULT res;
  int result;

  switch (pdrv) {
    case DEV_RAM:
      RAM_disk_write(buff, sector, count);
      return RES_OK;
    case DEV_FLASH:
      FLASH_disk_write(buff, sector, count);
      return RES_OK;
    default:
      return RES_PARERR;
  }
  return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
) {
  DRESULT res;
  int result;

  switch (pdrv) {
    case DEV_RAM:
      switch (cmd) {
        case GET_SECTOR_COUNT:
          *(LBA_t *)buff = RAM_disk_maxsector();
          return RES_OK;
        case GET_SECTOR_SIZE:
          *(WORD *)buff = RAM_disk_sectorsize();
          return RES_OK;
        case GET_BLOCK_SIZE:
          *(DWORD *)buff = 1;
          return RES_OK;
        case CTRL_SYNC:
          return RES_OK;
        default:
          return RES_PARERR;
      }
      return RES_PARERR;
    case DEV_FLASH:
      switch (cmd) {
        case GET_SECTOR_COUNT:
          *(LBA_t *)buff = FLASH_disk_maxsector();
          return RES_OK;
        case GET_SECTOR_SIZE:
          *(WORD *)buff = FLASH_disk_sectorsize();
          return RES_OK;
        case GET_BLOCK_SIZE:
          *(DWORD *)buff = 1;
          return RES_OK;
        case CTRL_SYNC:
          flush_page();
          return RES_OK;
        default:
          return RES_PARERR;
      }
      return RES_PARERR;
    default:
      break;
  }
  return RES_PARERR;
}
