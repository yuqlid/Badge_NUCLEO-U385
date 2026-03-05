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

GC9A01A tft1;

void APP_init() {
  printf("GC9A01A Init!\r\n");

  GC9A01A_init(&tft1, &hspi1, TFT1_CS_GPIO, TFT1_CS_PIN, TFT1_DC_GPIO,
               TFT1_DC_PIN, TFT1_BL_GPIO, TFT1_BL_PIN, TFT1_RST_GPIO,
               TFT1_RST_PIN);

  for (size_t i = 0; i < tft1.width; i += 1) {
    for (size_t j = 0; j < tft1.height; j += 1) {
      GC9A01A_draw_pixel(&tft1, j, i, 0xFFFF);
    }
  }
}

void Print_fixedImage() { GC9A01A_draw_screen(&tft1, (uint16_t *)image_data); }

void APP_main() { GC9A01A_draw_screen(&tft1, (uint16_t *)image_data); }
