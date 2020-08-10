###################################################
# Makefile for the NSS data plane driver
###################################################

obj ?= .

obj-$(CONFIG_QCOM_SW_HPPE) += switch-hppe.o

HPPE_SW_INCLUDE = -I$(obj)/include -I$(obj)/exports \
		  -I$(obj)/include/common -I$(obj)/include/sal/os \
		  -I$(obj)/include/sal/os/linux -I$(obj)/include/hsl \
		  -I$(obj)/include/init -I$(obj)/include/fal \
		  -I$(obj)/include/hsl/hppe/ -I$(obj)/include/ref

ccflags-y += $(HPPE_SW_INCLUDE)

switch-hppe-objs +=
	init/ssdk_clk.o
	init/ssdk_dts.o
	init/ssdk_hppe.o
	init/ssdk_init.o
	init/ssdk_interrupt.o
	init/ssdk_phy_i2c.o
	init/ssdk_plat.o
	init/ssdk_uci.o
