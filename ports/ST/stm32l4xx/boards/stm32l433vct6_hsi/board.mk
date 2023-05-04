CFLAGS += \
  -DSTM32L433xx \
  -DHSE_VALUE=8000000U 

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32l433xx.s
  
# For flash-jlink target
JLINK_DEVICE = stm32l433vc

flash: flash-jlink
erase: erase-jlink
