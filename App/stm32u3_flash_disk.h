/**
 * @file stm32u3_flash_disk.h
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef STM32U3_FLASH_DISK_H
#define STM32U3_FLASH_DISK_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define ADDR_FLASH_PAGE_128 \
  ((uint32_t)0x08080000) /* Base @ of Page 128, 4 Kbytes */
#define ADDR_FLASH_PAGE_255 \
  ((uint32_t)0x080FF000) /* Base @ of Page 255, 4 Kbytes */

#define SECTOR_SIZE 512

uint32_t GetPage(uint32_t Address);
uint32_t GetBank(uint32_t Address);

void chache_init(void);
void load_page(uint32_t page_index);
void flush_page(void);
void flash_read(unsigned char *buff, uint32_t sector, unsigned int count);
void flash_write(const unsigned char *buff, uint32_t sector,
                 unsigned int count);
uint32_t GetSectorSize(void);

#if defined(__cplusplus)
}
#endif

#endif  // STM32U3_FLASH_DISK_H