/**
 * @file cmd_flash.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "cmd_flash.hpp"

#include <charconv>
#include <cstdio>
#include <cstring>

#include "diskio.h"
#include "main.h"
#include "print_utils.h"
#include "stm32u3_flash_disk.h"

uint32_t DoubleWord[2] = {0x12345678, 0x12345678};
uint32_t Address = 0;
uint8_t test_date[SECTOR_SIZE];

#define ADDR_FLASH_PAGE_128 \
  ((uint32_t)0x08080000) /* Base @ of Page 128, 4 Kbytes */
#define FLASH_USER_START_ADDR ADDR_FLASH_PAGE_128

static void Cmd_FlashErase(EmbeddedCli *cli, char *args, void *context) {
  const char *arg1 = embeddedCliGetToken(args, 1);
  if (arg1 == nullptr) {
    printf("Erase page is required.\r\n");
    return;
  }
  uint8_t page{0};
  auto [ptr, ec] = std::from_chars(arg1, arg1 + std::strlen(arg1), page);
  if (ec != std::errc()) {
    printf(" invalid input value : %s\r\n", arg1);
    return;
  } else {
    printf(" input value : %d\r\n", page);
  }
  if (page >= 128) {
    printf(" page must be less than 128\r\n");
    return;
  }
  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks = FLASH_BANK_2;
  EraseInitStruct.Page = page;
  EraseInitStruct.NbPages = 1;
  uint32_t PageError = 0;
  HAL_FLASH_Unlock();
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
    printf(" Flash Erase Error : %lu\r\n", PageError);
  } else {
    printf(" Flash Erase Success : %d\r\n", page);
  }
  HAL_FLASH_Lock();
}

CliCommandBinding cmd_flashErase = {.name = "flasherase",
                                    .help = "Erase Flash Page",
                                    .tokenizeArgs = true,
                                    .context = NULL,
                                    .binding = Cmd_FlashErase};

static void Cmd_FlashWritePage(EmbeddedCli *cli, char *args, void *context) {
  const char *arg1 = embeddedCliGetToken(args, 1);
  if (arg1 == nullptr) {
    printf("Erase page is required.\r\n");
    return;
  }
  uint8_t page{0};
  auto [ptr, ec] = std::from_chars(arg1, arg1 + std::strlen(arg1), page);
  if (ec != std::errc()) {
    printf(" invalid input value : %s\r\n", arg1);
    return;
  } else {
    printf(" input value : %d\r\n", page);
  }
  if (page >= 128) {
    printf(" page must be less than 128\r\n");
    return;
  }

  Address = FLASH_USER_START_ADDR + page * FLASH_PAGE_SIZE;
  HAL_FLASH_Unlock();
  for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 8) {
    HAL_StatusTypeDef status = HAL_FLASH_Program(
        FLASH_TYPEPROGRAM_DOUBLEWORD, Address + i, (uint32_t)(&DoubleWord[0]));
    if (status != HAL_OK) {
      printf("HAL_State : %d Flash Program Error : 0x%lx\r\n", status,
             Address + i);
      break;
    }
  }
  printf(" Flash Write Finished : %d\r\n", page);
  HAL_FLASH_Lock();
}

CliCommandBinding cmd_flashWritePage = {.name = "flashwritepage",
                                        .help = "Write Flash Page",
                                        .tokenizeArgs = true,
                                        .context = NULL,
                                        .binding = Cmd_FlashWritePage};

static void Cmd_FlashWriteSector(EmbeddedCli *cli, char *args, void *context) {
  const char *arg1 = embeddedCliGetToken(args, 1);
  if (arg1 == nullptr) {
    printf("Write sector is required.\r\n");
    return;
  }
  uint8_t sector{0};
  auto [ptr, ec] = std::from_chars(arg1, arg1 + std::strlen(arg1), sector);
  if (ec != std::errc()) {
    printf(" invalid input value : %s\r\n", arg1);
    return;
  } else {
    printf(" input value : %d\r\n", sector);
  }
  if (sector >= 128) {
    printf(" sector must be less than 128\r\n");
    return;
  }
  for (uint16_t i = 0; i < SECTOR_SIZE; i++) {
    test_date[i] = (uint8_t)(sector + i);
  }
  FLASH_disk_write(test_date, sector, 1);
  flush_page();
  printf(" Flash Write Finished : %d\r\n", sector);
}

CliCommandBinding cmd_flashWriteSector = {.name = "flashwritesector",
                                          .help = "Write Flash Sector",
                                          .tokenizeArgs = true,
                                          .context = NULL,
                                          .binding = Cmd_FlashWriteSector};

static void Cmd_FlasDiskInfo(EmbeddedCli *cli, char *args, void *context) {
  disk_ioctl(1, GET_SECTOR_COUNT, &Address);
  printf(" Flash Disk Sector Count : %lu\r\n", Address);
  disk_ioctl(1, GET_SECTOR_SIZE, &Address);
  printf(" Flash Disk Sector Size  : %lu\r\n", Address);
}

CliCommandBinding cmd_flashDiskInfo = {.name = "flashdiskinfo",
                                       .help = "Show Flash Disk Info",
                                       .tokenizeArgs = true,
                                       .context = NULL,
                                       .binding = Cmd_FlasDiskInfo};
/**
 * @brief CLIコマンド登録
 *
 * @param cli
 */
void bindFlashCmds(EmbeddedCli *cli) {
  embeddedCliAddBinding(cli, cmd_flashErase);
  embeddedCliAddBinding(cli, cmd_flashWritePage);
  embeddedCliAddBinding(cli, cmd_flashWriteSector);
  embeddedCliAddBinding(cli, cmd_flashDiskInfo);
}