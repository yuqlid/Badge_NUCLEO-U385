/*
 * app.c
 *
 *  Created on: Jul 13, 2024
 *      Author: TK
 */

#include "app.h"

extern SPI_HandleTypeDef hspi1;

// load the weight data block from the model.bin file
INCLUDE_FILE(".rodata", "./img.bin", image);
extern uint8_t image_data[];
extern size_t image_start[];
extern size_t image_end[];

uint8_t counter;

GC9A01A tft1;

void APP_init() {
  printf("GC9A01A Test!\n");

  GC9A01A_init(&tft1, &hspi1, TFT1_CS_GPIO, TFT1_CS_PIN, TFT1_DC_GPIO,
               TFT1_DC_PIN, TFT1_BL_GPIO, TFT1_BL_PIN, TFT1_RST_GPIO,
               TFT1_RST_PIN);

  for (size_t i = 0; i < tft1.width; i += 1) {
    for (size_t j = 0; j < tft1.height; j += 1) {
      GC9A01A_draw_pixel(&tft1, j, i, 0xFFFF);
    }
  }
}

void APP_main() {
  //  char str[128];
  //  sprintf(str, "Done!\n");
  //  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);

   GC9A01A_draw_screen(&tft1, (uint16_t *)image_data);

  
  //  for (int i=0; i<120; i+=1) {
  //     GC9A01A_draw_pixel(&tft1, 120, i, color565(0, 128, 255));
  //  }
  //  for (int i=0; i<120; i+=1) {
  //    GC9A01A_draw_pixel(&tft1, i, 120, color565(255, 128, 0));
  //  }

  counter += 1;
}
