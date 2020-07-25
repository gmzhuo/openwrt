
include $(PRJ_PATH)/config
-include $(SYS_PATH)/include/config/auto.conf


ifndef OS_VER
  OS_VER=2_6
endif


  ifeq (ALL_CHIP, $(CHIP_TYPE))
     ifneq (TRUE, $(FAL))
         $(error FAL must be TRUE when CHIP_TYPE is defined as ALL_CHIP!)
     endif
     SUPPORT_CHIP = ISIS ISISC SHIVA DESS HPPE CPPE SCOMPHY
  endif

#TOOLPREFIX=aarch64-openwrt-linux-musl-

#define compile tool prefix
ifndef TOOLPREFIX
  TOOLPREFIX=$(CPU)-$(OS)-uclibc-
endif

DEBUG_ON=FALSE
OPT_FLAG=
LD_FLAG=

SHELLOBJ=ssdk_sh
US_MOD=ssdk_us
KS_MOD=ssdk_ks

ifeq (TRUE, $(KERNEL_MODE))
  RUNMODE=km
else
  RUNMODE=um
endif

BLD_DIR=$(PRJ_PATH)/build/$(OS)
BIN_DIR=$(PRJ_PATH)/build/bin

VER=2.0.0
BUILD_NUMBER=$(shell cat $(PRJ_PATH)/make/.build_number)
VERSION=$(VER).$(BUILD_NUMBER)
BUILD_DATE=$(shell date -u  +%F-%T)
