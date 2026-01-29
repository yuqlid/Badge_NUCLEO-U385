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

#include "print_utils.h"
#include "main.h"

uint32_t DoubleWord[2] = {0x12345678, 0x12345678};
uint32_t Address = 0;

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


static void Cmd_FlashWrite(EmbeddedCli *cli, char *args, void *context) {
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
    HAL_StatusTypeDef status =
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address + i,
                          (uint32_t)(&DoubleWord[0]));
    if (status != HAL_OK) {
      printf("HAL_State : %d Flash Program Error : %lx\r\n", status, Address + i);
      break;
    }
  }
  printf(" Flash Write Finished : %d\r\n", page);
  HAL_FLASH_Lock();
}

CliCommandBinding cmd_flashWrite = {.name = "flashwrite",
                                      .help = "Write Flash Page",
                                      .tokenizeArgs = true,
                                      .context = NULL,
                                      .binding = Cmd_FlashWrite};
/**
 * @brief CLIコマンド登録
 *
 * @param cli
 */
void bindFlashCmds(EmbeddedCli *cli) {
  embeddedCliAddBinding(cli, cmd_flashErase);
  embeddedCliAddBinding(cli, cmd_flashWrite);
}