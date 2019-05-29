INSTALL_DIR=/home/piuser/RFS/usr/bin/
CC=arm-linux-gnueabihf-gcc
CFLAGS=-g -O -Wall -lpthread


all: 				\
    crea_transac \
	client


install:
	sudo cp crea_transac ${INSTALL_DIR}
	sudo cp client ${INSTALL_DIR}

clean:
	rm -rf  crea_transac client
