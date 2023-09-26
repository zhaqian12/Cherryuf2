CFLAGS += \
	-DAT32F437ZMT7

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f435_437.s

# For flash-jlink target
JLINK_DEVICE = at32f437zmt7

flash: flash-jlink
erase: erase-jlink
