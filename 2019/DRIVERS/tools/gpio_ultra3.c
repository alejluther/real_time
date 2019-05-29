#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<sys/select.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<inttypes.h>

#include <sys/time.h>

#include "gpio_ioctl_rpi.h"

int main(int argc, char *argv[]){
    
    unsigned int dato;
    int fd_dev;
  
	if (argc != 2) {
		printf("Usage: %s dev_file\n", argv[0]);
		return -1;

	}
    fd_dev = open(argv[1], O_RDWR);
    if(fd_dev < 0){
        printf("Error opening device %s\n", argv[1]);
        return -ENODEV;
    }

	//printf("Sending GPIO_TRIGGER command\n");
    if(ioctl(fd_dev, GPIO_TRIGGER_ON, 0)){
        printf("Ioctl error for GPIO_TRIGGER_ON\n");
        return -1;
    }

	read(fd_dev, &dato, sizeof(int));
	printf("cm: %d\n", dato);

    return 0;
}
