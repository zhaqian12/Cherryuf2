CFLAGS += \
  -DAT32F405CCU7 \
  -DHEXT_VALUE=12000000U \
  -DSystemCoreClock=system_core_clock \
  -DCONFIG_USBDEV_MAX_BUS=1 \
  -DBOOTUF2_BUS_ID_HS=0x00

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f402_405.s
  
USB_XFER_SPEED = HS
