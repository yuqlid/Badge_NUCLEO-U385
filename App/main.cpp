/**
 * @file main.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-01-06
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "main.h"

#include <cstdio>

#include "app_usbx_device.h"
#include "build_info.hpp"
#include "cli/cmd_general.hpp"
#include "dma_driver.hpp"
#include "embedded_cli.h"
#include "gpdma.h"
#include "gpio.h"
#include "icache.h"
#include "print_utils.h"
#include "retain_config.hpp"
#include "spi.h"
#include "stm32u3_device.hpp"
#include "stm32u3_rcc_driver.hpp"
#include "usart.h"
#include "usb.h"

void SystemClock_Config(void);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
void __io_putchar(uint8_t ch) {
  while (!LL_USART_IsActiveFlag_TXE(USART1));
  LL_USART_TransmitData8(USART1, ch);
}

#ifdef __cplusplus
}
#endif

// embedded-cli関連
void writeChar(EmbeddedCli *embeddedCli, char c) { __io_putchar(c); }

Stm32u3Device sttm32u3device;
Stm32u3RccDriver stm32u3rccdriver;

static void print_boot_info(uint32_t csr) {
  printf("Hello World!\n\r");
  puts(build_time_str);
  puts(gcc_version_str);
  puts(git_branch_str);
  puts(git_tag_name_str);
  puts(git_info_str);

  sttm32u3device.printDeviceInfo();

  printf("SYSCLK : %3ld MHz\r\n", HAL_RCC_GetSysClockFreq() / 1000000);
  printf("HCLK   : %3ld MHz\r\n", HAL_RCC_GetHCLKFreq() / 1000000);
  printf("PCLK1  : %3ld MHz\r\n", HAL_RCC_GetPCLK1Freq() / 1000000);
  printf("PCLK2  : %3ld MHz\r\n", HAL_RCC_GetPCLK2Freq() / 1000000);
  printf("SCB->VTOR  : 0x%lx\r\n", SCB->VTOR);

  stm32u3rccdriver.printRCC_CSR(csr);
}

ringBufferWithDma<uint8_t, 32> uasart1_rx_ringbuff(
    GetDmaCBR1(GPDMA1, LL_DMA_CHANNEL_0));

int main(void) {
  retainConfigInit();
  uint32_t csr = stm32u3rccdriver.getRsr();
  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  unsigned int ret = MX_USBX_Device_Init();

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time
   * it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  print_boot_info(csr);

  if (ret == UX_SUCCESS) {
    print_green("USBX Device Initialized successfully.\r\n");
  } else {
    print_red("USBX Device Initialization failed.\r\n");
  }
  printf("MX_USBX_Device_Init() result : %d\r\n", ret);

  LinkDmaUsart1Rx(
      GPDMA1, LL_DMA_CHANNEL_0,
      reinterpret_cast<uint32_t>(uasart1_rx_ringbuff.bufferPointer()),
      uasart1_rx_ringbuff.bufferLength());
  LL_USART_EnableDMAReq_RX(USART1);
  LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);

  RetainConfig_t &retain_config = getRetainConfig();
  if (retain_config.reboot_count > 0) {
    printf("Reboot Count: %d.\r\n", retain_config.reboot_count);
  } else {
    printf("First boot.\r\n");
  }
  retain_config.reboot_count++;

  EmbeddedCliConfig *config = embeddedCliDefaultConfig();
  config->maxBindingCount = 30;
  EmbeddedCli *cli = embeddedCliNew(config);
  cli->writeChar = writeChar;
  bindGeneralCmds(cli);

  while (1) {

    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(1);
    
    while (!uasart1_rx_ringbuff.isEmpty()) {
      embeddedCliReceiveChar(cli,
                             static_cast<char>(uasart1_rx_ringbuff.dequeue()));
    }
    embeddedCliProcess(cli);
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK) {
    Error_Handler();
  }

  /** Enable Epod Booster
   */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS,
                                     RCC_EPODBOOSTER_DIV1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_PWREx_EnableEpodBooster() != HAL_OK) {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
   */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
   */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
