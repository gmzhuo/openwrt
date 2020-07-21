include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

define Device/Default
	PROFILES := Default
	KERNEL_DEPENDS = $$(wildcard $(DEVICE_DTS_DIR)/$$(DEVICE_DTS).dts)
	KERNEL_INITRAMFS_PREFIX := $$(IMG_PREFIX)-$(1)-initramfs
	KERNEL_PREFIX := $$(IMAGE_PREFIX)
	KERNEL_LOADADDR = 0x41080000
	DEVICE_DTS = $$(SOC)-$(lastword $(subst _, ,$(1)))
	SUPPORTED_DEVICES := $(subst _,$(comma),$(1))
	IMAGES := sysupgrade.bin
	IMAGE/sysupgrade.bin = sysupgrade-tar | append-metadata
	IMAGE/sysupgrade.bin/squashfs :=
endef

define Device/FitImage
	KERNEL_SUFFIX := -fit-uImage.itb
	KERNEL = kernel-bin | gzip | fit gzip $$(DTS_DIR)/$$(DEVICE_DTS).dtb
	KERNEL_NAME := Image
endef

define Device/UbiFit
	KERNEL_IN_UBI := 1
	IMAGES := nand-factory.bin nand-sysupgrade.bin
	IMAGE/nand-factory.bin := append-ubi
	IMAGE/nand-sysupgrade.bin := sysupgrade-tar | append-metadata
endef

define Device/xiaomi_ax3600
	$(call Device/FitImage)
	$(call Device/UbiFit)
	DEVICE_VENDOR := Xiaomi
	DEVICE_MODEL := AX3600
	DEVICE_VARIANT := standard
	DEVICE_DTS_CONFIG := config@ac04
	SOC := qcom-ipq8074
	KERNEL_INSTALL := 1
	DEVICE_DTS := qcom/ipq8074-ax3600
	BLOCKSIZE := 128k
	PAGESIZE := 2048
	BOARD_NAME := ax3600
	UBI_OPTS:="-m 2048 -p 128KiB -O 2048 -v"
endef
TARGET_DEVICES += xiaomi_ax3600

define Device/qcom_ipq8067-hk01
	$(call Device/FitImage)
	$(call Device/UbiFit)
	DEVICE_VENDOR := Qualcomm
	DEVICE_MODEL := HK01
	DEVICE_VARIANT := standard
	SOC := qcom-ipq8074
	KERNEL_INSTALL := 1
	DEVICE_DTS := qcom/ipq8074-hk01
	BLOCKSIZE := 128k
	PAGESIZE := 2048
	BOARD_NAME := hk01
endef
TARGET_DEVICES += qcom_ipq8067-hk01


$(eval $(call BuildImage))
