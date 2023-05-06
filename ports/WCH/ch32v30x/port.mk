UF2_FAMILY_ID ?= 0xf3d9b2c9
TOOLCHAIN_ROOT ?= /E/MounRiver/MounRiver_Studio/toolchain/RISC-VEmbeddedGCC/bin
CROSS_COMPILE = $(TOOLCHAIN_ROOT)/riscv-none-embed-

WCH_CMSIS = sdk/WCH/ch32v30x_library
WCH_DRIVERS = $(WCH_CMSIS)/Peripheral

# Port Compiler Flags
CFLAGS += \
  -flto \
  -march=rv32imac \
  -mabi=ilp32 \
  -msmall-data-limit=8 \
  -mno-save-restore \
  -fmessage-length=0 \
  -fsigned-char \
  -ffunction-sections \
  -nostdlib -nostartfiles 

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/CH32V30X_FLASH.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(WCH_CMSIS)/system_ch32v30x.c \
	$(WCH_CMSIS)/ch32v30x_it.c \
	$(WCH_CMSIS)/Core/core_riscv.c \
	$(WCH_CMSIS)/Debug/debug.c \
	$(WCH_DRIVERS)/src/ch32v30x_flash.c \
	$(WCH_DRIVERS)/src/ch32v30x_gpio.c \
	$(WCH_DRIVERS)/src/ch32v30x_misc.c \
	$(WCH_DRIVERS)/src/ch32v30x_rcc.c

# Port include
INC += \
	$(TOP)/$(WCH_CMSIS) \
	$(TOP)/$(WCH_CMSIS)/Core \
	$(TOP)/$(WCH_CMSIS)/Debug \
	$(TOP)/$(WCH_DRIVERS)/inc

# Port source for USB
SRC_C += $(CHERRYUSB_DIR)/port/ch32/usb_dc_usbfs.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/ch32
