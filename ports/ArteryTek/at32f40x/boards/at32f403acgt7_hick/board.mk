CFLAGS += \
  	-DAT32F403ACGT7

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f403a_407.s

# For flash-jlink target
JLINK_DEVICE = at32f403acgt7

flash: flash-jlink
erase: erase-jlink
