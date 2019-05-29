INSTALL_DIR=/home/piuser/RFS/usr/bin/
CC=arm-linux-gnueabihf-gcc
CFLAGS=-g -O -Wall


all: 				\
    virt_dev_read \
    virt_dev_read_block \
    virt_dev_write \
    virt_dev_bad_write \
    virt_dev_ioctl \
    virt_dev_select \
    gpio_mmap \
    gpio_ultra \
    gpio_ultra2 \
    gpio_ultra3 


install:
	sudo cp virt_dev_read ${INSTALL_DIR}
	sudo cp virt_dev_read_block ${INSTALL_DIR}
	sudo cp virt_dev_write ${INSTALL_DIR}
	sudo cp virt_dev_bad_write ${INSTALL_DIR}
	sudo cp virt_dev_select ${INSTALL_DIR}
	sudo cp virt_dev_ioctl ${INSTALL_DIR}
	sudo cp gpio_mmap ${INSTALL_DIR}
	sudo cp gpio_ultra ${INSTALL_DIR}
	sudo cp gpio_ultra2 ${INSTALL_DIR}
	sudo cp gpio_ultra3 ${INSTALL_DIR}
	sudo cp script_trace.sh  /home/piuser/RFS/home/

clean:
	rm -rf  virt_dev_read virt_dev_read_block virt_dev_write virt_dev_ioctl gpio_ultra \
			gpio_mmap gpio_ultra2 gpio_ultra3 virt_dev_bad_write virt_dev_select
