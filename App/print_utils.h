/**
 * @file print_utils.h
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-09-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PRINT_UTILS_H_
#define PRINT_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/**
 * @brief Print a green colored string
 *
 * @param str
 */
static inline void print_green(const char *str) {
  printf(
      "\x1b[32m%s\x1b[0m",
      str);  // ANSIエスケープシーケンスで緑色に設定してから文字列を出力し、最後にリセット
}

/**
 * @brief Print a yellow colored string
 *
 * @param str
 */
static inline void print_yellow(const char *str) {
  printf(
      "\x1b[33m%s\x1b[0m",
      str);  // ANSIエスケープシーケンスで黄色に設定してから文字列を出力し、最後にリセット
}

/**
 * @brief Print a red colored string
 *
 * @param str
 */
static inline void print_red(const char *str) {
  printf(
      "\x1b[31m%s\x1b[0m",
      str);  // ANSIエスケープシーケンスで赤色に設定してから文字列を出力し、最後にリセット
}

#ifdef __cplusplus
}
#endif

#endif /* PRINT_UTILS_H_ */