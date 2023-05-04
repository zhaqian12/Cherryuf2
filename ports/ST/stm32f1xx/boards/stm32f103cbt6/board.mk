CFLAGS += \
  -DSTM32F103xB \
  -DHSE_VALUE=8000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f103xb.s
  
# For flash-jlink target
JLINK_DEVICE = stm32f103cb

flash: flash-jlink
erase: erase-jlink
