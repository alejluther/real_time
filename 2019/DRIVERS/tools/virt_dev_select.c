#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>

int
main(int argc, char *argv[])
{

	int fd1, fd2;
	fd_set virt_dev_read_set;
	int ret;
	int dato;
	char filename_1[30];
	char filename_2[30];

	if (argc != 2) {
		printf("Usage: %s device\n", argv[0]);
		return -1;
	}

	memset(filename_1, '\0', 30);
	sprintf(filename_1, "%s0", argv[1]);

	memset(filename_2, '\0', 30);
	sprintf(filename_2, "%s1", argv[1]);

	fd1 = open(filename_1, O_RDONLY);
	if (fd1 < 0) {
		printf("Error abriendo fichero %s\n", filename_1);
		return -1;
	}

	fd2 = open(filename_2, O_RDONLY);
	if (fd2 < 0) {
		printf("Error abriendo fichero %s\n", filename_2);
		return -1;
	}

	while(1){
		FD_ZERO(&virt_dev_read_set);
		FD_SET(fd1, &virt_dev_read_set);
		FD_SET(fd2, &virt_dev_read_set);
		ret = select(fd2+1, &virt_dev_read_set, 0, 0, 0);
		if (ret == 0)
			break;

		printf("Select return %d\n", ret);
		if (FD_ISSET(fd1, &virt_dev_read_set)) {
			read(fd1, &dato, sizeof(int));
			printf("Getting data %d from 0\n", dato);
		}

		if (FD_ISSET(fd2, &virt_dev_read_set)) {
			read(fd2, &dato, sizeof(int));
			printf("Getting data %d from 1\n", dato);
		}

	}
	return 0;
}
