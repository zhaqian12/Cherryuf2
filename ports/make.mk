# ---------------------------------------
# Common make definition for all
# ---------------------------------------

CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

ifneq ($(lastword a b),b)
$(error This Makefile require make 3.81 or newer)
endif

# Set TOP to be the path to get from the current directory (where make was
# invoked) to the top of the tree. $(lastword $(MAKEFILE_LIST)) returns
# the name of this makefile relative to where make was invoked.
THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
TOP := $(shell realpath $(THIS_MAKEFILE))
TOP := $(patsubst %/ports/make.mk,%,$(TOP))

CURRENT_PATH := $(shell realpath --relative-to=$(TOP) `pwd`)

#-------------- Handy check parameter function ------------
check_defined = \
    $(strip $(foreach 1,$1, \
    $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
    $(error Undefined: $1$(if $2, ($2))))

#-------------- Select the board to build for. ------------

# PORT is default to directory name containing the Makefile
# can be set manually by custom build such as flash_nuke
PORT ?= $(notdir $(shell pwd))
PORT_DIR = ports/$(PORTS)/$(PORT)
BOARD_DIR = $(PORT_DIR)/boards/$(BOARD)

ifeq ($(wildcard $(TOP)/$(BOARD_DIR)/),)
  $(info You must provide a BOARD parameter with 'BOARD=')
  $(error Invalid BOARD specified)
endif

# Build directory
BUILD = .build/$(BOARD)
BIN = $(TOP)/$(PORT_DIR)/_bin/$(BOARD)

# can be set manually by custom build such as flash_nuke
OUTNAME ?= cherryuf2_$(BOARD)
GIT_SUBMODULES ?= CherryUSB

# UF2 version with git tag and submodules
GIT_VERSION := $(shell git describe --dirty="-dev" --always --tags)

CFLAGS += \
  -DCONFIG_BOOTUF2_FAMILYID=$(UF2_FAMILY_ID) \
  -DCONFIG_VERSION='"$(GIT_VERSION)"'

DEPFLAGS ?=
#-------------- Bootloader --------------

SRC_C += \
  src/bootuf2_desc.c \
  src/bootuf2.c \
  src/main.c

INC += \
  $(TOP)/src \
  $(TOP)/$(PORT_DIR) \
  $(TOP)/$(BOARD_DIR)

#-------------- CherryUSB --------------
VALID_USB_XFER_SPEED := FS HS FS_HS
USB_XFER_SPEED ?= FS

CHERRYUSB_DIR = lib/CherryUSB

SRC_C += \
	$(CHERRYUSB_DIR)/core/usbd_core.c \
	$(CHERRYUSB_DIR)/class/msc/usbd_msc.c 

INC += \
  $(TOP)/$(CHERRYUSB_DIR)/core \
  $(TOP)/$(CHERRYUSB_DIR)/common \
  $(TOP)/$(CHERRYUSB_DIR)/class/msc 

#-------------- Common Compiler Flags --------------

# Compiler Flags
CFLAGS += \
  -Os\
  -ggdb \
  -fdata-sections \
  -ffunction-sections \
  -fsingle-precision-constant \
  -fno-strict-aliasing \
  -Wdouble-promotion \
  -Wstrict-prototypes \
  -Wextra \
  -Wall \
  -Wfatal-errors \
  -Werror-implicit-function-declaration \
  -Wfloat-equal \
  -Wundef \
  -Wshadow \
  -Wwrite-strings \
  -Wsign-compare \
  -Wmissing-format-attribute \
  -Wunreachable-code \
  -Wcast-align \
  -DCONFIG_USBDEV_ADVANCE_DESC

# Linker Flags
LDFLAGS += \
	-fshort-enums \
	-Wl,-Map=$@.map \
	-Wl,-cref \
	-Wl,-gc-sections

# libc
LIBS += -lgcc -lm -lc

LDFLAGS += -specs=nosys.specs -specs=nano.specs
LIBS += -lnosys

# Board specific define
# TODO should be moved to port.mk
include $(TOP)/$(BOARD_DIR)/board.mk

ifeq ($(filter $(USB_XFER_SPEED),$(VALID_USB_XFER_SPEED)),)
  $(call Invalid USB_XFER_SPEED, USB_XFER_SPEED="$(USB_XFER_SPEED)" is not a valid value)
else
  CFLAGS += -DUSB_XFER_USE_$(strip $(shell echo $(USB_XFER_SPEED) | tr '[:lower:]' '[:upper:]'))
endif

