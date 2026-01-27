/**
 * @file stm32u3_flash.c
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "stm32u3_flash.h"

#include "stm32u3xx_hal.h"

static uint8_t page_cache[FLASH_PAGE_SIZE];
static uint32_t cached_page_index = 0xFFFFFFFF;
static uint8_t cache_dirty = 0;


/**
 * @brief  Gets the page of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The page of a given address
 */
uint32_t GetPage(uint32_t Addr) {
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  } else {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
 * @brief  Gets the bank of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The bank of a given address
 */
uint32_t GetBank(uint32_t Addr) {
  uint32_t bank = 0;

  if (READ_BIT(FLASH->OPTR, FLASH_OPTR_SWAP_BANK) == 0) {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
      bank = FLASH_BANK_1;
    } else {
      bank = FLASH_BANK_2;
    }
  } else {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
      bank = FLASH_BANK_2;
    } else {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}

void chache_init(void) {
  cached_page_index = 0xFFFFFFFF;
  cache_dirty = 0;
}

void load_page(uint32_t page_index) {
  uint32_t addr = ADDR_FLASH_PAGE_128 + page_index * FLASH_PAGE_SIZE;
  memcpy(page_cache, (void *)addr, FLASH_PAGE_SIZE);
  cached_page_index = page_index;
}

void flush_page(void) {
  if (!cache_dirty || cached_page_index == 0xFFFFFFFF) return;

  uint32_t page_addr =
      ADDR_FLASH_PAGE_128 + cached_page_index * FLASH_PAGE_SIZE;

  HAL_FLASH_Unlock();

  FLASH_EraseInitTypeDef erase = {

      .TypeErase = FLASH_TYPEERASE_PAGES,
      .Banks = FLASH_BANK_2,
      .Page = (page_addr - FLASH_BASE) / FLASH_PAGE_SIZE,
      .NbPages = 1};
  uint32_t error;
  HAL_FLASHEx_Erase(&erase, &error);

  for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 8) {
    uint64_t data;
    memcpy(&data, &page_cache[i], 8);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_addr + i, data);
  }

  HAL_FLASH_Lock();
  cache_dirty = 0;
}

void flash_read(unsigned char *buff, uint32_t sector, unsigned int count) {
  while (count--) {
    uint32_t addr = ADDR_FLASH_PAGE_128 + sector * SECTOR_SIZE;
    memcpy(buff, (void *)addr, SECTOR_SIZE);
    buff += SECTOR_SIZE;
    sector += 1;
  }
}

void flash_write(const unsigned char *buff, uint32_t sector,
                 unsigned int count) {
  while (count--) {
    uint32_t page_index = sector / (FLASH_PAGE_SIZE / SECTOR_SIZE);
    uint32_t offset = (sector % (FLASH_PAGE_SIZE / SECTOR_SIZE)) * SECTOR_SIZE;

    if (page_index != cached_page_index) {
      flush_page();
      load_page(page_index);
    }

    memcpy(&page_cache[offset], buff, SECTOR_SIZE);
    cache_dirty = 1;

    buff += SECTOR_SIZE;
    sector += 1;
  }
}

uint32_t GetSectorSize(void) { return SECTOR_SIZE; }