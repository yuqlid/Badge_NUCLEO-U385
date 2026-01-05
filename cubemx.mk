
# CubeMXの自動生成ファイルの取り込み

CUBEMX = Badge_NUCLEO-U385_CubeMX

C_INCLUDES +=  \
-I$(CUBEMX)/Core/Inc \
-I$(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Inc \
-I$(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Inc/Legacy \
-I$(CUBEMX)/BSP/STM32U3xx_Nucleo \
-I$(CUBEMX)/Drivers/CMSIS/Device/ST/STM32U3xx/Include \
-I$(CUBEMX)/Drivers/CMSIS/Include \


C_SOURCES +=  \
$(CUBEMX)/Core/Src/gpio.c \
$(CUBEMX)/Core/Src/icache.c \
$(CUBEMX)/Core/Src/stm32u385xx_hal_msp.c \
$(CUBEMX)/Core/Src/stm32u385xx_it.c \
$(CUBEMX)/Core/Src/system_stm32u385xx.c \
$(CUBEMX)/Core/Src/sysmem.c \
$(CUBEMX)/Core/Src/syscalls.c \
$(wildcard $(CUBEMX)/Drivers/STM32U3xx_HAL_Driver/Src/*.c) \

# AS includes
AS_INCLUDES +=  \
-I$(CUBEMX)/Core/Inc

# ASM sources
ASM_SOURCES =  \
$(CUBEMX)/startup_stm32u385xx.s.s

LINKERSCRIPT =  \
$(CUBEMX)/STM32U385xx_FLASH.ld