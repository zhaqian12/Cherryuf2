CFLAGS += \
  -DSTM32G431xx
  
SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32g431xx.s

USB_XFER_SPEED = FS