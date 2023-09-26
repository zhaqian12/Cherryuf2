CFLAGS += \
  -DAT32F405CCU7 \
  -DHEXT_VALUE=12000000U\
  -DCONFIG_USB_HS

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f402_405.s
  
USB_XFER_SPEED = HS

# For flash-jlink target
JLINK_DEVICE = at32f405ccu7

flash: flash-jlink
erase: erase-jlink
