CFLAGS += \
  -DAT32WB415CCU7_7\
  -DHEXT_VALUE=8000000U

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32wb415.s

