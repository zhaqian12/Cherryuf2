CFLAGS += \
  -DSTM32F072xB \
  -DHSE_VALUE=8000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f072xb.s
  
# For flash-jlink target
JLINK_DEVICE = stm32f072cb

flash: flash-jlink
erase: erase-jlink
