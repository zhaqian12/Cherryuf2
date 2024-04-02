CFLAGS += \
  -DCH582M \
  -DUSB_BASE=0x40008000U

SRC_S += \
  $(WCH_CMSIS)/Startup/startup_CH583.S
