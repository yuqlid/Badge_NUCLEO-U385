/**
 * @file retain_config.cpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-12-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "retain_config.hpp"

#include <cstring>

__attribute__((section(".noinit"), used)) static RetainConfig_t retain;

RetainConfig_t& getRetainConfig() { return retain; }

void retainConfigInit() {
  constexpr uint32_t kRetainMagic = 0xDEADBEF;

  if (retain.magic != kRetainMagic) {
    memset(&retain, 0, sizeof(retain));
    retain.magic = kRetainMagic;
  }
}