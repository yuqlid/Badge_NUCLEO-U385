/*
 * app.h
 *
 *  Created on: Jul 13, 2024
 *      Author: TK
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gc9a01a.h"
#include "stm32u3xx_hal.h"
#include "stm32u3xx_ll_gpio.h"

// http://elm-chan.org/junk/32bit/binclude.html
#define INCLUDE_FILE(section, filename, symbol) asm (\
    ".section "#section"\n"                   /* Change section */\
    ".balign 4\n"                             /* Word alignment */\
    ".global "#symbol"_start\n"               /* Export the object start address */\
    ".global "#symbol"_data\n"                /* Export the object address */\
    #symbol"_start:\n"                        /* Define the object start address label */\
    #symbol"_data:\n"                         /* Define the object label */\
    ".incbin \""filename"\"\n"                /* Import the file */\
    ".global "#symbol"_end\n"                 /* Export the object end address */\
    #symbol"_end:\n"                          /* Define the object end address label */\
    ".balign 4\n"                             /* Word alignment */\
    ".section \".text\"\n")                   /* Restore section */



// Chip select
#define TFT1_CS_GPIO   GPIOA
#define TFT1_CS_PIN    LL_GPIO_PIN_4

// Data/command
#define TFT1_DC_GPIO   GPIOC
#define TFT1_DC_PIN    LL_GPIO_PIN_7

// Backlight control
#define TFT1_BL_GPIO   GPIOC
#define TFT1_BL_PIN    LL_GPIO_PIN_8

#define TFT1_RST_GPIO  GPIOC
#define TFT1_RST_PIN   LL_GPIO_PIN_9




void APP_init();

void APP_main();

void Print_fixedImage();

#if defined(__cplusplus)
}
#endif

#endif /* INC_APP_H_ */
