CFLAGS += \
  -DSTM32F072xB \
  -DBOARD_VECTOR_REDIRECT_BOOTLOADER
  
SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f072xb.s

USB_XFER_SPEED = FS
