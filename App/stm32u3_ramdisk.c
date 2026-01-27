/**
 * @file stm32u3_ramdisk.c
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-27
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "stm32u3_ramdisk.h"

#include <string.h>

#define BLOCK_SIZE 512

#define RAM_DISK_SIZE (128 * 1024)

uint8_t ram_disk[RAM_DISK_SIZE];

void RAM_disk_initialize(void) {
  // Initialize the RAM disk to zero
  //memset(ram_disk, 0, sizeof(ram_disk));
}

void RAM_disk_read(unsigned char *buff, uint32_t sector, unsigned int count) {
  uint32_t offset = sector * BLOCK_SIZE;
  uint32_t size = count * BLOCK_SIZE;
  
  //__disable_irq();
  for (uint32_t i = 0; i < size; i++) {
    buff[i] = ram_disk[offset + i];
  }
  //__enable_irq();
}

void RAM_disk_write(const unsigned char *buff, uint32_t sector,
                    unsigned int count) {
  uint32_t offset = sector * BLOCK_SIZE;
  uint32_t size = count * BLOCK_SIZE;
  
  //__disable_irq();
  for (uint32_t i = 0; i < size; i++) {
    ram_disk[offset + i] = buff[i];
  }
  //__enable_irq();
}

uint32_t RAM_disk_maxsector(void) { return RAM_DISK_SIZE / BLOCK_SIZE; }
uint16_t RAM_disk_sectorsize(void) { return BLOCK_SIZE; }