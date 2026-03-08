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
#include "gc9a01a.h"

extern GC9A01A tft1;
extern uint8_t ram_disk[];

static unsigned int read;
static FATFS fs;

TCHAR path[] = "1:";  // Path to the drive (flash disk)

static void Cmd_mount(EmbeddedCli *cli, char *args, void *context) {
  FRESULT res = f_mount(&fs, path, 1);
  if (res != FR_OK) {
    printf("Failed to mount filesystem: %d\r\n", res);
    return;
  }

  f_chdrive(path);  // Change current drive to 1 (flash disk)
                    // 相対パスを指定するために必要
  printf("Filesystem mounted successfully\r\n");
}

CliCommandBinding cmd_mount = {.name = "mount",
                               .help = "Mount filesystem",
                               .tokenizeArgs = false,
                               .context = NULL,
                               .binding = Cmd_mount};

static void Cmd_readfile(EmbeddedCli *cli, char *args, void *context) {
  FIL file;
  FRESULT res = f_open(&file, "img2.bin", FA_READ);
  if (res != FR_OK) {
    printf("Failed to open file: %d\r\n", res);
    return;
  }

  GC9A01A_set_windows_size(&tft1);

  f_read(&file, ram_disk, 240 * 240 * 2, &read);
  printf("Read %u bytes from file\r\n", read);
  GC9A01A_transmit_data(
      &tft1, 240 * 240,
      reinterpret_cast<uint16_t *>(ram_disk));  // Push one row

  f_close(&file);
}

CliCommandBinding cmd_readfile = {.name = "readfile",
                                  .help = "Read file from filesystem",
                                  .tokenizeArgs = false,
                                  .context = NULL,
                                  .binding = Cmd_readfile};

static void Cmd_ls(EmbeddedCli *cli, char *args, void *context) {
  FRESULT res;
  DIR dir;
  FILINFO fno;

  res = f_opendir(&dir, path);
  if (res != FR_OK) {
    printf("opendir error: %d\n", res);
    return;
  }

  while (1) {
    res = f_readdir(&dir, &fno);
    if (res != FR_OK || fno.fname[0] == 0) {
      break;
    }

    if (fno.fattrib & AM_DIR) {
      printf("<DIR>  %s\r\n", fno.fname);
    } else {
      printf("%8lu %s\r\n", fno.fsize, fno.fname);
    }
  }

  f_closedir(&dir);
}

CliCommandBinding cmd_ls = {.name = "ls",
                            .help = "List files in directory",
                            .tokenizeArgs = false,
                            .context = NULL,
                            .binding = Cmd_ls};

void bindFatfsCmds(EmbeddedCli *cli) {
  embeddedCliAddBinding(cli, cmd_mount);
  embeddedCliAddBinding(cli, cmd_readfile);
  embeddedCliAddBinding(cli, cmd_ls);
}
