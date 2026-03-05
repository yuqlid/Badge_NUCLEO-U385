/**
 * @file cmd_fatfs.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-03-05
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "cmd_fatfs.hpp"

#include <charconv>
#include <cstdio>
#include <cstring>

#include "ff.h"

static void Cmd_mount(EmbeddedCli *cli, char *args, void *context) {
  FATFS fs;
  FRESULT res = f_mount(&fs, "1:", 1);  // Mount the filesystem on drive 1 (flash disk)
  if (res != FR_OK) {
    printf("Failed to mount filesystem: %d\r\n", res);
    return;
  }

  f_chdrive("1:");  // Change current drive to 1 (flash disk) 相対パスを指定するために必要
  FIL file;

  res = f_open(&file, "img.bin", FA_READ);
  if (res != FR_OK) {
    printf("Failed to open file: %d\r\n", res);
    return;
  }
}

CliCommandBinding cmd_mount = {.name = "mount",
                               .help = "Mount filesystem",
                               .tokenizeArgs = false,
                               .context = NULL,
                               .binding = Cmd_mount};

void bindFatfsCmds(EmbeddedCli *cli) { embeddedCliAddBinding(cli, cmd_mount); }
