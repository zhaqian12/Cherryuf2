CFLAGS += \
  -DAT32F423CCU7

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f423.s
  
USB_XFER_SPEED = FS
