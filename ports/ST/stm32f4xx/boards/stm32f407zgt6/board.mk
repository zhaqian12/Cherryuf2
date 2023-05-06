CFLAGS += \
  -DSTM32F407xx \
  -DHSE_VALUE=25000000U \

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f407xx.s

LD_FILES = $(PORT_DIR)/ld/STM32F407XG_FLASH.ld

# For flash-jlink target
JLINK_DEVICE = stm32f407zg

flash: flash-jlink
erase: erase-jlink
