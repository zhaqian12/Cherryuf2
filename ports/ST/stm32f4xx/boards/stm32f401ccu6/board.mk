CFLAGS += \
  -DSTM32F401xC \
  -DHSE_VALUE=25000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f401xc.s
  
LD_FILES = $(PORT_DIR)/ld/STM32F401XC_FLASH.ld

USB_XFER_SPEED = FS
