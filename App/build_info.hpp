/**
 * @file build_info.hpp
 * @author KUSAKABE Yuki (yuqid@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BUILD_INFO_HPP_
#define BUILD_INFO_HPP_

#include "fw_buildenv.h"

/**
 * @brief ビルド情報
 * 一連の値はMakefileで定義されているものとする
 * そうでないときはfw_buildenv.h内部のundefinedが呼び出される
 */
constexpr char build_time_str[] = "\r\n" __DATE__", " __TIME__ "\r";
constexpr char gcc_version_str[] = "gcc version : " ARM_GCC_VERSION "\r";
constexpr char git_branch_str[] = "git branch : " GIT_BRANCH_NAME "\r";
constexpr char git_tag_name_str[] = "git tag : " GIT_TAG_NAME "\r";
constexpr char git_info_str[] = "git info : " GIT_BUILD_INFO "\r";

#endif /* NTCTHERMISTOR_HPP_ */