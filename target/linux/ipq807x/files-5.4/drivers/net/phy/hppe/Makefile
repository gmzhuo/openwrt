###################################################
# Makefile for the NSS data plane driver
###################################################

obj ?= .

obj-$(CONFIG_QCOM_SW_HPPE) += switch-hppe.o

switch-hppe-objs += main.o ssdk_plat.o hsl/hppe_acl.o \
		hsl/hppe_reg_access.o

HPPE_SW_INCLUDE = -I$(obj)/include -I$(obj)/exports \
		  -I$(obj)/include/common -I$(obj)/include/sal/os \
		  -I$(obj)/include/sal/os/linux -I$(obj)/include/hsl \
		  -I$(obj)/include/init -I$(obj)/include/fal \
		  -I$(obj)/include/hsl/hppe/ -I$(obj)/include/ref

ccflags-y += $(HPPE_SW_INCLUDE)

