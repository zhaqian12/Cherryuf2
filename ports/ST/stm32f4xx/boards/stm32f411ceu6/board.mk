CFLAGS += \
  -DSTM32F411xE \
  -DHSE_VALUE=8000000U \

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f411xe.s

LD_FILES = $(PORT_DIR)/ld/STM32F411XE_FLASH.ld

# For flash-jlink target
JLINK_DEVICE = stm32f411ce

flash: flash-jlink
erase: erase-jlink
