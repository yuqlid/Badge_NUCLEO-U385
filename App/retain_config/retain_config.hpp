/**
 * @file retain_config.hpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-12-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef RETAIN_CONFIG_HPP_
#define RETAIN_CONFIG_HPP_

#include <cstdint>

struct RetainConfig_t {
  uint32_t magic;
  uint32_t reboot_count;
  uint32_t error_code;
};

RetainConfig_t& getRetainConfig();
void retainConfigInit();

#endif  // RETAIN_CONFIG_HPP_