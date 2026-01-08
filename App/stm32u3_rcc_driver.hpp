/**
 * @file stm32h5_rcc_driver.hpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef STMC32H5_RCC_DRIVER_HPP_
#define STMC32H5_RCC_DRIVER_HPP_

#include <cstdio>
#include "stm32u3xx_ll_rcc.h"

class Stm32u3RccDriver {
 public:
  const char* getStringRCC_CSR(uint32_t csr) {
    switch (csr) {
      case RCC_CSR_LPWRRSTF:
        return "Low-power reset";
        break;
      case RCC_CSR_WWDGRSTF:
        return "Window watchdog reset";
        break;
      case RCC_CSR_IWDGRSTF:
        return "Independent watchdog reset";
        break;
      case RCC_CSR_SFTRSTF:
        return "Software reset";
        break;
      case RCC_CSR_BORRSTF:
        return "BOR reset";
        break;
      case RCC_CSR_PINRSTF:
        return "Pin reset";
        break;
      default:
        return "Invalid value";
        break;
    }
  }

  void printRCC_CSR(uint32_t csr) {
    if (LL_RCC_IsActiveFlag_LPWRRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_LPWRRSTF));
    if (LL_RCC_IsActiveFlag_WWDGRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_WWDGRSTF));
    if (LL_RCC_IsActiveFlag_IWDGRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_IWDGRSTF));
    if (LL_RCC_IsActiveFlag_SFTRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_SFTRSTF));
    if (LL_RCC_IsActiveFlag_BORRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_BORRSTF));
    if (LL_RCC_IsActiveFlag_PINRST())
      printf("%s\r\n", getStringRCC_CSR(RCC_CSR_PINRSTF));
  }
  uint32_t getRsr() { return RCC->CSR & 0xFA000000; }
  void resetFlags() { LL_RCC_ClearResetFlags(); }
};

#endif /* STMC32H5_RCC_DRIVER_HPP_ */