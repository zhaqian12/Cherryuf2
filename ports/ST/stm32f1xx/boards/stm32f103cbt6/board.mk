CFLAGS += \
  -DSTM32F103xB \
  -DHSE_VALUE=8000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f103xb.s

