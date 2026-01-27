/**
 * @file stm32u3_ramdisk.h
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-27
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef STM32U3_RAMDISK_H
#define STM32U3_RAMDISK_H

#include <stdint.h>

void RAM_disk_initialize(void);

void RAM_disk_read(unsigned char *buff, uint32_t sector, unsigned int count);

void RAM_disk_write(const unsigned char *buff, uint32_t sector,
                    unsigned int count);

uint32_t RAM_disk_maxsector(void);
uint16_t RAM_disk_sectorsize(void);

#endif /* STM32U3_RAMDISK_H */
