CROSS_COMPILE=arm-linux-gnueabihf-

obj-m := driver_01.o

KDIR := /home/piuser/software/kernel/linux
PWD:= $(shell pwd)

default:
	$(MAKE) ARCH=arm -C $(KDIR)  SUBDIRS=$(PWD) modules

clean:
	rm -rf *.ko *.o .driver* .tmp* *mod.c Module.symvers modules.order
