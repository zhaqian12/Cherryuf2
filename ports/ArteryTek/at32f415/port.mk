UF2_FAMILY_ID ?= 0x6a1b09b4
CROSS_COMPILE = arm-none-eabi-

AT_DRIVERS = sdk/ArteryTek/at32f415_library/libraries/drivers
AT_CMSIS = sdk/ArteryTek/at32f415_library/libraries/cmsis

# Port Compiler Flags
CFLAGS += \
  -flto \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m4 \
  -mfloat-abi=soft \
  -nostdlib -nostartfiles

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/AT32F415xB_FLASH.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(AT_CMSIS)/cm4/device_support/system_at32f415.c \
	$(AT_DRIVERS)/src/at32f415_crm.c\
	$(AT_DRIVERS)/src/at32f415_flash.c\
	$(AT_DRIVERS)/src/at32f415_gpio.c\
	$(AT_DRIVERS)/src/at32f415_misc.c

# Port include
INC += \
	$(TOP)/$(AT_CMSIS)/cm4/core_support \
	$(TOP)/$(AT_CMSIS)/cm4/device_support \
	$(TOP)/$(AT_DRIVERS)/inc

# Port source for USB
SRC_C += $(CHERRYUSB_DIR)/port/dwc2/usb_dc_dwc2.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/dwc2

DEPFLAGS += -include $(TOP)/$(AT_CMSIS)/cm4/device_support/at32f415.h