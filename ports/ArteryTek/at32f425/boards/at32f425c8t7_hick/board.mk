CFLAGS += \
  -DAT32F425C8T7

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f425.s
  
# For flash-jlink target
JLINK_DEVICE = at32f425c8t7

flash: flash-jlink
erase: erase-jlink
