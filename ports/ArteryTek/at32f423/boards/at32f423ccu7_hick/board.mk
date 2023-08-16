CFLAGS += \
  -DAT32F423CCU7

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f423.s
  
# For flash-jlink target
JLINK_DEVICE = at32f423ccu7

flash: flash-jlink
erase: erase-jlink
