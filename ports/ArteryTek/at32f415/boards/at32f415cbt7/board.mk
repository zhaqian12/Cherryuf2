CFLAGS += \
  -DAT32F415CBT7 \
  -DHEXT_VALUE=8000000U

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f415.s
  
# For flash-jlink target
JLINK_DEVICE = at32f415cbt7

flash: flash-jlink
erase: erase-jlink
