/**
 * @file cmdlist_general.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "cmd_general.hpp"

#include <cstdio>

#include "stm32u3_device.hpp"

static void onLed(EmbeddedCli *cli, char *args, void *context) {
  // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
}

CliCommandBinding led_binding = {.name = "led",
                                 .help = "Toggle led status",
                                 .tokenizeArgs = true,
                                 .context = NULL,
                                 .binding = onLed};

static void Cmd_Reboot(EmbeddedCli *cli, char *args, void *context) {
  __disable_fault_irq();
  __disable_irq();
  // NVIC_DisableIRQ();
  NVIC_SystemReset();
}

CliCommandBinding cmd_Reboot = {.name = "reboot",
                                .help = "Reboot System",
                                .tokenizeArgs = false,
                                .context = NULL,
                                .binding = Cmd_Reboot};

/**
 * @brief CLIコマンド登録
 *
 * @param cli
 */
void bindGeneralCmds(EmbeddedCli *cli) {
  embeddedCliAddBinding(cli, led_binding);
  embeddedCliAddBinding(cli, cmd_Reboot);
}