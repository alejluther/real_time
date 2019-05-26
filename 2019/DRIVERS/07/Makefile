CROSS_COMPILE=arm-linux-gnueabihf-

obj-m := driver_07.o

KDIR := /home/piuser/software/kernel/linux
PWD:= $(shell pwd)

default:
	$(MAKE) ARCH=arm -C $(KDIR)  SUBDIRS=$(PWD) modules

install:
	sudo cp driver_07.ko /home/piuser/RFS/home/

clean:
	rm -rf *.ko *.o .driver* .tmp* *mod.c Module.symvers modules.order
