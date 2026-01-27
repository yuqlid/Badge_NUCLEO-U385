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
  // memset(ram_disk, 0, sizeof(ram_disk));
}

void RAM_disk_read(unsigned char *buff, uint32_t sector, unsigned int count) {
  memcpy(buff, &ram_disk[sector * BLOCK_SIZE], count * BLOCK_SIZE);
}

void RAM_disk_write(const unsigned char *buff, uint32_t sector,
                    unsigned int count) {
  memcpy(&ram_disk[sector * BLOCK_SIZE], buff, count * BLOCK_SIZE);
}

uint32_t RAM_disk_maxsector(void) { return RAM_DISK_SIZE / BLOCK_SIZE; }
uint16_t RAM_disk_sectorsize(void) { return BLOCK_SIZE; }