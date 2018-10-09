obj-m := kernel_thread.o

KDIR := /home/piuser/software/kernel/linux
PWD:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR)  SUBDIRS=$(PWD) modules

clean:
	rm -rf *.ko *.o .driver* .tmp* *mod.c Module.symvers modules.order rm .kernel*
