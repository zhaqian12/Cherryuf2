CFLAGS += \
	-DAT32F437ZMT7 \
	-DAT32_USE_OTGFS2 \
	-DOTG_VBUS_IGNORE

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f435_437.s
