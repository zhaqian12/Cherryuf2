CFLAGS += \
  -DSTM32L433xx

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32l433xx.s

USB_XFER_SPEED = FS