/**
 * @file stm32u3_device.hpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef STM32U3_DEVICE_HPP_
#define STM32U3_DEVICE_HPP_

#include <cstdio>

#include "stm32u3xx_ll_utils.h"

class Stm32u3Device
{
public:
    void printDeviceInfo()
    {
        printf("UID0 : %08lx\r\n", LL_GetUID_Word0());
        printf("UID1 : %08lx\r\n", LL_GetUID_Word1());
        printf("UID2 : %08lx\r\n", LL_GetUID_Word2());
        printf("Flash size: %ld kB\r\n", LL_GetFlashSize());
        printf("Package: %s\r\n", getStringDevicePackage());
        printf("FPU: %s\r\n", getStringFPU());
    }

    const char *getStringDevicePackage()
    {
        switch (LL_GetPackageType())
        {
        case LL_UTILS_PACKAGETYPE_UQFN32:
            return "UQFN32";
            break;
        case LL_UTILS_PACKAGETYPE_UQFN48:
            return "UQFN48";
            break;
        case LL_UTILS_PACKAGETYPE_BGA59:
            return "BGA59";
            break;
        case LL_UTILS_PACKAGETYPE_UQFN32_SMPS:
            return "UQFN32_SMPS";
            break;
        case LL_UTILS_PACKAGETYPE_UQFN48_SMPS:
            return "UQFN48_SMPS";
            break;
        default:
            return "Invalid value";
            break;
        }
    }

    const char *getStringFPU()
    {
        switch (SCB_GetFPUType())
        {
        case 0:
            return "No FPU";
            break;
        case 1:
            return "Single precision FPU";
            break;
        default:
            return "Invalid value";
            break;
        }
    }
};

#endif /* STM32U3_DEVICE_HPP_ */