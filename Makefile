DEFS+=-DMYDEV_CDEV=1

#DEFS+=-DCONFIG_DEBUG_INFO_BTF_MODULES=0
#DEFS+=-DCONFIG_DEBUG_INFO_BTF=n
#DEFS+=-DCONFIG_MODULE_FORCE_LOAD=y

ifneq (${KERNELRELEASE},)
        obj-m  = esp32_usbled.o
else
        KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
        MODULE_DIR := $(shell pwd)

.PHONY: all

all: modules

.PHONY:modules

modules:
	${MAKE} -C ${KERNEL_DIR} EXTRA_CFLAGS+='${DEFS}' M=${MODULE_DIR}  modules

clean:
	rm -f *.o *.ko *.mod .*.o .*.ko *.mod.c .*.cmd *~
	rm -f Module.symvers Module.markers modules.order
	rm -rf .tmp_versions
endif
