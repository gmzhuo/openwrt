#
# Copyright (C) 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ath11k-firmware
PKG_SOURCE_DATE:=2019-10-03
PKG_SOURCE_VERSION:=master
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/kvalo/ath11k-firmware.git

PKG_MAINTAINER:=New Maintainer <maintainer@maintainer.com>

include $(INCLUDE_DIR)/package.mk

define Package/ath11k-firmware-default
  SECTION:=firmware
  CATEGORY:=Firmware
  URL:=$(PKG_SOURCE_URL)
  DEPENDS:=
endef

define Package/ath11k-firmware-ipq8074
$(Package/ath11k-firmware-default)
  TITLE:=ath11k firmware for ipq8074 devices
endef

define Build/Compile

endef

define Package/ath11k-firmware-ipq8074/install
	$(INSTALL_DIR) $(1)/lib/firmware/IPQ8074
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/m3_fw.b00 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/m3_fw.b01 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/m3_fw.b02 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/m3_fw.flist $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/m3_fw.mdt $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b00 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b01 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b02 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b03 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b04 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b05 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b07 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.b08 $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.flist $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.mdt $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/2.1.0.1/WLAN.HK.2.1.0.1-01238-QCAHKSWPL_SILICONZ-2/q6_fw.mdt $(1)/lib/firmware/IPQ8074/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/IPQ8074/hw2.0/board-2.bin $(1)/lib/firmware/IPQ8074/
endef


$(eval $(call BuildPackage,ath11k-firmware-ipq8074))
