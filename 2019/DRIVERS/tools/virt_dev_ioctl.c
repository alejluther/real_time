#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "virt_dev_ioctl.h"

int
main(int argc, char *argv[]) {

	int fich, command;
	char filename[20];

	if (argc < 3) {
		printf("Usage: %s device command\n\t\n\tcommand:\n\t\t0: VIRT_DEV_RESET\n\t\t1: VIRT_DEV_SET_ONLY_EVEN\n\t\t2: VIRT_SET_UPPER_LIMIT\n", argv[0]);
		return -1;
	}

	command = atoi(argv[2]);

	memset(filename, '\0', 20);
	sprintf(filename, "%s", argv[1]);

	fich = open(filename, O_RDWR);
	if (fich < 0) {
		printf("Fallo al abrir device %s\n", filename);
		return -1;
	}

	if (command == 0) {
		if (ioctl(fich, VIRT_DEV_RESET, NULL) != 0) {
			printf("Ioctl error: VIRT_DEV_RESET failed\n");
			return -1;
		}
	}

	if (command == 1) {
		if (ioctl(fich, VIRT_DEV_SET_ONLY_EVEN) != 0) {
			printf("Ioctl error: VIRT_DEV_SET_ONLY_EVEN failed\n");
			return -1;
		}
	}

	if (command == 2) {
		unsigned long limit;

		if (argc < 4) {
			printf("Mode VIRT_SET_UPPER_LIMIT requires the limit value\n");
			close(fich);
			return -1;
		}

		limit = (unsigned long)atoi(argv[3]);
		if (limit < 0) {
			printf("The limit needs to be positive\n");
			close(fich);
			return -1;
		}
		
		if (ioctl(fich, VIRT_DEV_SET_UPPER_LIMIT, &limit) != 0) {
			printf("Ioctl error: VIRT_DEV_UPPER_LIMIT failed\n");
			return -1;
		}
	}

	close(fich);
	return 0;
}
