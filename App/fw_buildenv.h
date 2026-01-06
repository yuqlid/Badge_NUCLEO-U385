/*
 * fw_buildenv.h
 *
 *  Created on: 2022/09/28
 *      Author: Yuki Kusakabe
 */

#ifndef FW_BUILDENV_H_
#define FW_BUILDENV_H_

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef ARM_GCC_VERSION
#define ARM_GCC_VERSION "undefined"
#endif

#ifndef GIT_BRANCH_NAME
#define GIT_BRANCH_NAME "undefined"
#endif

#ifndef GIT_TAG_NAME
#define GIT_TAG_NAME "undefined"
#endif

#ifndef GIT_BUILD_INFO
#define GIT_BUILD_INFO "undefined"
#endif

#ifndef BUILD_DATE
#define BUILD_DATE "undefined"
#endif

#ifndef BUILD_MACHINE
#define BUILD_MACHINE "undefined"
#endif

#ifdef __cplusplus
}
#endif

#endif /* FW_BUILDENV_H_ */
