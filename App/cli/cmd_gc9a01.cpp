/**
 * @file cmd_gc9A01.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-02-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "cmd_gc9a01.hpp"

#include <charconv>
#include <cstdio>
#include <cstring>

#include "app.h"
#include "main.h"

static void Cmd_BackLight(EmbeddedCli *cli, char *args, void *context) {
  const char *arg1 = embeddedCliGetToken(args, 1);
  if (arg1 == nullptr) {
    printf("brightness (0-100) required\r\n");
    return;
  }
  int level = 0;
  auto [ptr, ec] = std::from_chars(arg1, arg1 + std::strlen(arg1), level);
  if (ec != std::errc() || level < 0 || level > 100) {
    printf("invalid brightness value: %s\r\n", arg1);
    return;
  }
  uint16_t ccr1 = (uint16_t)((level * LL_TIM_GetAutoReload(TIM3)) / 100);
  LL_TIM_OC_SetCompareCH1(TIM3, ccr1);
  printf("backlight set to %d%% (compare=%u)\r\n", level, ccr1);
}

CliCommandBinding cmd_backlight = {.name = "bl",
                                   .help = "Control BackLight",
                                   .tokenizeArgs = false,
                                   .context = NULL,
                                   .binding = Cmd_BackLight};
static void Cmd_UpdateImage(EmbeddedCli *cli, char *args, void *context) {
  printf("Updating image...\r\n");
  Print_fixedImage();
}

CliCommandBinding cmd_updateimage = {.name = "update",
                                     .help = "Update Image",
                                     .tokenizeArgs = false,
                                     .context = NULL,
                                     .binding = Cmd_UpdateImage};

void bindGC9A01Cmds(EmbeddedCli *cli) {
  embeddedCliAddBinding(cli, cmd_backlight);
  embeddedCliAddBinding(cli, cmd_updateimage);
}
