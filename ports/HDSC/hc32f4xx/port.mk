UF2_FAMILY_ID ?= 0x12639409
CROSS_COMPILE = arm-none-eabi-

HC_DRIVERS = sdk/HDSC/hc32f4xx_library/hc32_ll_driver
HC_CMSIS = sdk/HDSC/hc32f4xx_library/cmsis

# Port Compiler Flags
CFLAGS += \
  -flto \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m4 \
  -mfloat-abi=hard \
  -mfpu=fpv4-sp-d16 \
  -nostdlib -nostartfiles \
  -DUSE_DDL_DRIVER

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/HC32F460xE.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(HC_CMSIS)/Device/HDSC/hc32f4xx/Source/system_hc32f460.c \
	$(HC_DRIVERS)/src/hc32_ll.c \
	$(HC_DRIVERS)/src/hc32_ll_icg.c \
	$(HC_DRIVERS)/src/hc32_ll_utility.c \
	$(HC_DRIVERS)/src/hc32_ll_sram.c \
	$(HC_DRIVERS)/src/hc32_ll_pwc.c \
	$(HC_DRIVERS)/src/hc32_ll_clk.c \
	$(HC_DRIVERS)/src/hc32_ll_interrupts.c \
	$(HC_DRIVERS)/src/hc32_ll_gpio.c \
	$(HC_DRIVERS)/src/hc32_ll_efm.c \
	$(HC_DRIVERS)/src/hc32_ll_fcg.c \
	lib/SEGGER_RTT/RTT/SEGGER_RTT.c \
	lib/SEGGER_RTT/RTT/SEGGER_RTT_printf.c
	
# Port include
INC += \
	$(TOP)/$(HC_CMSIS)/Device/HDSC/hc32f4xx/Include \
	$(TOP)/$(HC_CMSIS)/Include \
	$(TOP)/$(HC_DRIVERS)/inc \
	$(TOP)/lib/SEGGER_RTT/RTT

# Port source for USB
SRC_C += $(CHERRYUSB_DIR)/port/dwc2/usb_dc_dwc2.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/dwc2