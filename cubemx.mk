
# CubeMXの自動生成ファイルの取り込み

CUBEMX = Badge_NUCLEO-U385_CubeMX

C_INCLUDES +=  \
-I$(CUBEMX)/Core/Inc \
-I$(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Inc \
-I$(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Inc/Legacy \
-I$(CUBEMX)/Drivers/BSP/STM32U3xx_Nucleo \
-I$(CUBEMX)/Drivers/CMSIS/Device/ST/STM32U3xx/Include \
-I$(CUBEMX)/Drivers/CMSIS/Include \
-I$(CUBEMX)/USBX/App \
-I$(CUBEMX)/USBX/Target \
-I$(CUBEMX)/Middlewares/ST/usbx/common/core/inc \
-I$(CUBEMX)/Middlewares/ST/usbx/ports/generic/inc \
-I$(CUBEMX)/Middlewares/ST/usbx/common/usbx_stm32_device_controllers \
-I$(CUBEMX)/Middlewares/ST/usbx/common/usbx_device_classes/inc \



C_SOURCES +=  \
$(CUBEMX)/Drivers/BSP/STM32U3xx_Nucleo/stm32u3xx_nucleo.c \
$(wildcard $(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Src/*.c) \
$(wildcard $(CUBEMX)/Core/Src/*.c) \
$(wildcard $(CUBEMX)/USBX/App/*.c) \
$(wildcard $(CUBEMX)/Middlewares/ST/usbx/common/core/src/*.c) \
$(wildcard $(CUBEMX)/Middlewares/ST/usbx/common/usbx_device_classes/src/*.c ) \
$(wildcard $(CUBEMX)/Middlewares/ST/usbx/common/usbx_stm32_device_controllers/*.c ) \

# AS includes
AS_INCLUDES +=  \
-I$(CUBEMX)/Core/Inc

# ASM sources
ASM_SOURCES =  \
$(CUBEMX)/startup_stm32u385xx.s

LINKERSCRIPT =  \
$(CUBEMX)/STM32U385xx_FLASH.ld