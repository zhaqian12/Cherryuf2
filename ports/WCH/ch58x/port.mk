UF2_FAMILY_ID ?= 0x42030ad6
TOOLCHAIN_ROOT ?= /E/MounRiver/MounRiver_Studio/toolchain/RISC-VEmbeddedGCC/bin
CROSS_COMPILE = $(TOOLCHAIN_ROOT)/riscv-none-embed-

WCH_CMSIS = sdk/WCH/ch58x_library
WCH_DRIVERS = $(WCH_CMSIS)/StdPeriphDriver

# Port Compiler Flags
CFLAGS += \
  -flto \
  -march=rv32i \
  -mabi=ilp32 \
  -msmall-data-limit=8 \
  -mno-save-restore \
  -fmessage-length=0 \
  -fsigned-char \
  -ffunction-sections \
  -fdata-sections \
  -nostdlib -nostartfiles 

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/CH58X_FLASH.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(WCH_CMSIS)/RVMSIS/core_riscv.c \
	$(WCH_DRIVERS)/CH58x_clk.c \
	$(WCH_DRIVERS)/CH58x_flash.c \
	$(WCH_DRIVERS)/CH58x_gpio.c \
	$(WCH_DRIVERS)/CH58x_sys.c

# Port include
INC += \
	$(TOP)/$(WCH_CMSIS)/RVMSIS \
	$(TOP)/$(WCH_DRIVERS)/inc

# Port lib
LIBS += $(TOP)/$(WCH_DRIVERS)/libISP583.a

# Port source for USB
SRC_C += $(CHERRYUSB_DIR)/port/ch32/usb_ch58x_dc_usbfs.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/ch32
