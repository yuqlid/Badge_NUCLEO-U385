
######################################
# target
######################################
TARGET = Badgo_NUCLEO-U385
#STATICLIB_PREFIX = lib

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX= $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
DP = $(GCC_PATH)/$(PREFIX)objdump
else
CC = $(PREFIX)gcc
CXX= $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
DP = $(PREFIX)objdump
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
LST = $(DP) -D -Sd

######################################
# build version info
######################################

# get arm-none-eabi-gcc version
ARM_GCC_VERSION := $(shell $(CC) -dumpversion)
# get current git branch
GIT_BRANCH_NAME := $(shell git rev-parse --abbrev-ref HEAD)
# get current commit hash(short)
GIT_COMMIT_HASH := $(shell git rev-parse --short HEAD)

# Reference : https://embeddedartistry.com/blog/2016/12/21/giving-your-firmware-build-a-version/
version := $(subst -, ,$(shell git describe --long --dirty --tags))
COMMIT := $(strip $(word 3, $(version)))
COMMITS_PAST := $(strip $(word 2, $(version)))
DIRTY := $(strip $(word 4, $(version)))
ifneq ($(COMMITS_PAST), 0)
	BUILD_INFO_COMMITS := "."$(COMMITS_PAST)
endif
ifneq ($(DIRTY),)
	BUILD_INFO_DIRTY :="+"
endif

export BUILD_TAG :=$(strip $(word 1, $(version)))
export BUILD_INFO := $(COMMIT)$(BUILD_INFO_COMMITS)$(BUILD_INFO_DIRTY)

ifeq ($(SHELL), cmd.exe)
BUILD_DATE := $(shell python -c "from datetime import datetime; print(datetime.utcnow().strftime('%d/%m/%Y, %H:%M'))"
BUILD_MACHINE := $(shell echo %username%)@$(shell hostname)
else
BUILD_MACHINE := $(shell whoami)@$(shell hostname)
#BUILD_DATE := $(shell date -u +"%d/%m/%Y, %H:%M")
BUILD_DATE := $(shell date +"%Y/%m/%d, %H:%M")
endif


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
# Build path
LIB_DIR = Lib

######################################
# source
######################################

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES +=  \
-IApp \
-Iembedded-cli/lib/include \
-IFatFs/source \
-IApp
#-IApp/crc \
#-IMiddlewares/EEPROM_Emul/Core \
#-IMiddlewares/EEPROM_Emul/Porting  \
#-IApp/eeprom_emul \

C_INCLUDES +=  \
#-Iesi \

# C sources
C_SOURCES =  \
embedded-cli/lib/src/embedded_cli.c \
FatFs/source/ff.c \
FatFs/source/diskio.c \
FatFs/source/ffsystem.c \
FatFs/source/ffunicode.c \
App/stm32u3_ram_disk.c \
App/stm32u3_flash_disk.c \


# CubeMXで生成したファイルの読み込み
# ASM_SOURCES,LINKERSCRIPT,はcubemx.mkで定義済み
include cubemx.mk

# ASM sources
ASM_UPPER_SOURCES = \


# C++ includes
CPP_INCLUDES = $(C_INCLUDES)
CPP_INCLUDES += \
-IApp \
-IApp/retain_config

# C++ sources
CPP_SOURCES = \
App/main.cpp \
App/dma_driver.cpp \
App/retain_config/retain_config.cpp

CPP_SOURCES += \
App/cli/cmd_general.cpp \
App/cli/cmd_flash.cpp \
#App/cli/cmd_drv8316cr.cpp \
#App/cli/cmd_voltage.cpp \
#App/cli/cmd_eeprom.cpp \
#App/cli/cmd_tim1_test.cpp \
#App/cli/cmd_current.cpp \
#App/cli/cmd_calibration.cpp\

# CppUTestのソースを読み込む
#include cpputestlib.mk

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m33
# fpu
FPU = -mfpu=fpv4-sp-d16
# float-abi
FLOAT-ABI = -mfloat-abi=hard
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_NUCLEO_64 \
-DUSE_HAL_DRIVER \
-DSTM32U385xx \
-DUSE_FULL_LL_DRIVER \
-DUX_INCLUDE_USER_DEFINE_FILE

C_DEFS +=  \
-DARM_GCC_VERSION=\"$(ARM_GCC_VERSION)\" \
-DGIT_BRANCH_NAME=\"$(GIT_BRANCH_NAME)\" \
-DGIT_TAG_NAME=\"$(BUILD_TAG)\" \
-DGIT_BUILD_INFO=\"$(BUILD_INFO)\" \
-DBUILD_DATE=\""$(BUILD_DATE)"\" \
-DBUILD_MACHINE=\""$(BUILD_MACHINE)"\"

# for EEPROM Emulation defines
# どちらか一つだけ定義する
#C_DEFS +=  \
#-DEDATA_ENABLED
#-DFLASH_LINES_128B

# C++ defines
CPP_DEFS = $(C_DEFS)

# C++ defines for CppUTest
CPPUTEST_DEFS += $(CPP_DEFS)

# float option
# -Wfloat-conversion : doubleからfloatへ暗黙の型変換が発生したときに警告
FLOAT-OPT += -Wfloat-conversion
# -Wdouble-promotion : 内部演算でflaotからdoubleへ暗黙の型変換が発生したときに警告
FLOAT-OPT += -Wdouble-promotion
# -fsingle-precision-constant : 浮動小数点の計算は全て単精度（float）で実装
FLOAT-OPT += -fsingle-precision-constant
# -Wfloat-equal : 浮動小数点で==,!=の比較を使っているときに警告
FLOAT-OPT += -Wfloat-equal

WARN_STACK_SIZE = 256

OPT += -fstack-usage -Wstack-usage=$(WARN_STACK_SIZE)
#OPT += -fcyclomatic-complexity
OPT += -Wall
OPT += -fdata-sections
OPT += -ffunction-sections
OPT += -fcommon
#OPT += -Wconversion

COMMONFLAGS = $(OPT) $(FLOAT-OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
COMMONFLAGS += -g -gdwarf-2
endif
COMMONFLAGS += -fdebug-prefix-map=/=

# Generate dependency information
COMMONFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
# -Winline : inline指定している関数がinline展開されなかったときに警告
#COMMONFLAGS += -Winline
# -Wdisabled-optimization : コンパイラが最適化できなかったときに警告
COMMONFLAGS += -Wdisabled-optimization

ifeq ($(OPTIMIZATION),-O0)
else
# -fno-inline : 関数インライン展開を抑制
COMMONFLAGS += -fno-inline
endif

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(COMMONFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(COMMONFLAGS)

# compile g++ flags
CPPFLAGS = $(MCU) $(CPP_DEFS) $(CPP_INCLUDES) $(COMMONFLAGS)

#CCACHE := $(shell which ccache)

# C++のみのオプション
# -fno-use-cxa-atexit	プログラム終了時、デストラクタを呼ぶための __cxa_atexit の呼び出し(とリンク)を禁止する
# -fno-rtti	実時間情報を無効化
# -fno-exceptions		例外無効化
# -Werror=return-type 戻り値のある関数なのにreturnを設定していないとエラーとする

CPPFLAGS += -fno-rtti -fno-exceptions -fno-use-cxa-atexit
CPPFLAGS += -std=c++17
CPPFLAGS += -Werror=return-type

# Cのみのオプション
# -Wunsuffixed-float-constants : 浮動小数点の定数に型を示すサフィックスがないことを警告
CFLAGS += -Wunsuffixed-float-constants
CFLAGS += -std=gnu11

CPPUTESTFLAGS = $(MCU) $(CPPUTEST_DEFS) $(CPPUTEST_INCLUDES) $(COMMONFLAGS) -fno-rtti -fno-exceptions -fno-use-cxa-atexit -std=c++17
#CPPUTESTFLAGS += --specs=nano.specs
#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(LINKERSCRIPT)

# libraries
LIBS = -lc -lm -lnosys
LIBS += -lstdc++ -lsupc++
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,-print-memory-usage
LDFLAGS += -u _printf_float
# default action: build all
#all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin checksum $(BUILD_DIR)/$(TARGET).lst
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of c++ objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_UPPER_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_UPPER_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
$(BUILD_DIR):
	mkdir $@		

dump: $(BUILD_DIR)/$(TARGET).lst

#######################################
# objdump
#######################################
# Target for generating objdump file
$(BUILD_DIR)/$(TARGET).lst: $(BUILD_DIR)/$(TARGET).elf
	$(LST) $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).lst

#######################################
# checksum
#######################################
checksum: $(BUILD_DIR)/$(TARGET).hex
	srec_cat $(BUILD_DIR)/$(TARGET).hex -Intel -fill 0xFF 0x08000000 0x0803F7FC -STM32 0x0803F7FC -o $(BUILD_DIR)/$(TARGET)_crc.hex -Intel
	$(CP) -I ihex -O binary -S $(BUILD_DIR)/$(TARGET)_crc.hex $(BUILD_DIR)/$(TARGET)_crc.bin

checksumgen: $(BUILD_DIR)/$(TARGET).hex
	srec_cat $(BUILD_DIR)/$(TARGET).hex -Intel -fill 0xFF 0x08000000 0x0803F7FC -STM32 0x0803F7FC -crop 0x0803F7FC 0x0803F800 -o - -hex_dump

#######################################
# Program CubrProg
#######################################
upload: $(BUILD_DIR)/$(TARGET).bin
	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/$(TARGET).bin 0x08000000 --verify -rst

#upload: $(BUILD_DIR)/$(TARGET)_crc.bin
#	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/$(TARGET)_crc.hex 0x08000000 --verify -rst
#	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/$(TARGET)_crc.bin 0x08000000 --verify -rst

#######################################
# Program OpenOCD
#######################################
upload_cube: $(BUILD_DIR)/$(TARGET).hex
	openocd -f board/st_nucleo_g4.cfg -c "reset_config trst_only combined" -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)


#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


#######################################
# build static_library
#######################################

# 中間ファイルを削除するターゲットを定義
clean_lib_intermediate:
	-rm -f $(LIB_DIR)/*.o
	-rm -f $(LIB_DIR)/*.su
	-rm -f $(LIB_DIR)/*.d
	-rm -f $(LIB_DIR)/*.lst

#lib: $(LIB_DIR)/$(STATICLIB_PREFIX)CppUTest.a clean_lib_intermediate

# list of c++ objects
#CPPUTESTLIBOBJECTS = $(addprefix $(LIB_DIR)/,$(notdir $(CPPUTEST_SOURCES:.cpp=.o)))
#vpath %.cpp $(sort $(dir $(CPPUTEST_SOURCES)))

#$(LIB_DIR)/%.o: %.cpp Makefile | $(LIB_DIR) 
#	$(CXX) -c $(CPPUTESTFLAGS) -Wa,-a,-ad,-alms=$(LIB_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

#$(LIB_DIR)/$(STATICLIB_PREFIX)CppUTest.a: $(LIB_DIR) $(CPPUTESTLIBOBJECTS)
#	$(AR) -cr $@ $(CPPUTESTLIBOBJECTS)

$(LIB_DIR):
	mkdir -p $@

#######################################
# clean up static_library
#######################################

clean_lib:
	-rm -fR $(LIB_DIR)

# clean_lib_intermediateがlibターゲットの前に実行されることを防ぐために追加
.PHONY: clean_lib_intermediate


# *** EOF ***
