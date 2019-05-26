INSTALL_DIR=/home/piuser/RFS/usr/bin/
CC=arm-linux-gnueabihf-gcc
CFLAGS=-g -O -Wall


all: 				\
    virt_dev_read \
    virt_dev_read_block \
    virt_dev_write \
    virt_dev_bad_write \
    virt_dev_ioctl \
    virt_dev_select 


install:
	sudo cp virt_dev_read ${INSTALL_DIR}
	sudo cp virt_dev_read_block ${INSTALL_DIR}
	sudo cp virt_dev_write ${INSTALL_DIR}
	sudo cp virt_dev_bad_write ${INSTALL_DIR}
	sudo cp virt_dev_ioctl ${INSTALL_DIR}
	sudo cp ultra_ctrl ${INSTALL_DIR}
	sudo cp script_trace.sh  /home/piuser/RFS/home/

clean:
	rm -rf  virt_dev_read virt_dev_read_block virt_dev_write virt_dev_ioctl ultra_ctrl \
		    virt_dev_bad_write virt_dev_select
