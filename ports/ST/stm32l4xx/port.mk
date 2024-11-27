UF2_FAMILY_ID ?= 0x00ff6919
CROSS_COMPILE = arm-none-eabi-

ST_DRIVERS ?= sdk/ST/stm32l4xx_library/stm32l4xx_hal_driver
ST_CMSIS ?= sdk/ST/stm32l4xx_library/cmsis_device_l4
CMSIS_5 ?= sdk/CMSIS_5

# Port Compiler Flags
CFLAGS += \
  -flto \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m4 \
  -mfloat-abi=hard \
  -mfpu=fpv4-sp-d16 \
  -nostdlib -nostartfiles \
  -DUSE_HAL_DRIVER

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/STM32L433XC_FLASH.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(ST_CMSIS)/Source/Templates/system_stm32l4xx.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_cortex.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_rcc.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_rcc_ex.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_pwr.c\
	$(ST_DRIVERS)/Src/stm32l4xx_hal_pwr_ex.c\
	$(ST_DRIVERS)/Src/stm32l4xx_hal_gpio.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_flash.c \
	$(ST_DRIVERS)/Src/stm32l4xx_hal_flash_ex.c

# Port include
INC += \
	$(TOP)/$(CMSIS_5)/CMSIS/Core/Include \
	$(TOP)/$(ST_CMSIS)/Include \
	$(TOP)/$(ST_DRIVERS)/Inc

# Port source for USB
SRC_C += $(CHERRYUSB_DIR)/port/fsdev/usb_dc_fsdev.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/fsdev
