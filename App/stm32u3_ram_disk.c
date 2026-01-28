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

#include "stm32u3_ram_disk.h"

#include <string.h>

#define BLOCK_SIZE 512

#define RAM_DISK_SIZE (128 * 1024)

static uint8_t ram_disk[RAM_DISK_SIZE];

/**
 * @brief Initialize the RAM disk.
 */
void RAM_disk_initialize(void) {
  // Initialize the RAM disk to zero
  memset(ram_disk, 0, sizeof(ram_disk));
}

/**
 * @brief Read data from the RAM disk.
 *
 * @param buff Pointer to the buffer to read into.
 * @param sector Starting sector number to read from.
 * @param count Number of sectors to read.
 */
void RAM_disk_read(unsigned char *buff, uint32_t sector, unsigned int count) {
  memcpy(buff, &ram_disk[sector * BLOCK_SIZE], count * BLOCK_SIZE);
}

/**
 * @brief Write data to the RAM disk.
 *
 * @param buff Pointer to the data buffer to write from.
 * @param sector Starting sector number to write to.
 * @param count Number of sectors to write.
 */
void RAM_disk_write(const unsigned char *buff, uint32_t sector,
                    unsigned int count) {
  memcpy(&ram_disk[sector * BLOCK_SIZE], buff, count * BLOCK_SIZE);
}

/**
 * @brief Get the maximum sector number of the RAM disk.
 *
 * @return uint32_t The maximum sector number.
 */
uint32_t RAM_disk_maxsector(void) { return RAM_DISK_SIZE / BLOCK_SIZE; }

/**
 * @brief Get the sector size of the RAM disk.
 *
 * @return uint16_t The sector size in bytes.
 */
uint16_t RAM_disk_sectorsize(void) { return BLOCK_SIZE; }