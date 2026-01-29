/**
 * @file stm32u3_flash_disk.c
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "stm32u3_flash_disk.h"

#include "stm32u3xx_hal.h"

static uint8_t page_cache[FLASH_PAGE_SIZE];  // 1Page(4kB) cache
static uint32_t cached_page_index = 0xFFFFFFFF;
static uint8_t cache_dirty = 0;

#define FLASH_DISK_SIZE (128 * 1024)

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
void FLASH_disk_initialize(void) { chache_init(); }
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

  uint32_t global_page = (page_addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  uint32_t bank_relative_page = global_page;
  if (GetBank(page_addr) == FLASH_BANK_2) {
    bank_relative_page -= FLASH_PAGE_NB;
  }

  FLASH_EraseInitTypeDef erase = {

      .TypeErase = FLASH_TYPEERASE_PAGES,
      .Banks = GetBank(page_addr),
      .Page = bank_relative_page,
      .NbPages = 1};
  uint32_t error;
  HAL_FLASHEx_Erase(&erase, &error);

  for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 8) {
    /* DataAddress should point to the source data in page_cache */
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_addr + i,
                      (uint32_t)(&page_cache[i]));
  }

  HAL_FLASH_Lock();
  cache_dirty = 0;
}

void FLASH_disk_read(unsigned char *buff, uint32_t sector, unsigned int count) {
  while (count--) {
    uint32_t addr = ADDR_FLASH_PAGE_128 + sector * SECTOR_SIZE;
    memcpy(buff, (void *)addr, SECTOR_SIZE);
    buff += SECTOR_SIZE;
    sector += 1;
  }
}

void FLASH_disk_write(const unsigned char *buff, uint32_t sector,
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

uint32_t FLASH_disk_maxsector(void) { return FLASH_DISK_SIZE / SECTOR_SIZE; }

uint16_t FLASH_disk_sectorsize(void) { return SECTOR_SIZE; }