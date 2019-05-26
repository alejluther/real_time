#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int fich, data, res;
	char filename[20];

	if (argc != 3) {
		printf("Usage: %s device data\n", argv[0]);
		return -1;
	}

	memset(filename, '\0', 20);
	sprintf(filename, "%s", argv[1]);

	fich = open(filename, O_RDWR);
	if (fich < 0) {
		printf("Opening file %s failed\n", filename);
		return -1;
	}

	data = atoi(argv[2]);
	res = write(fich, &data, 4);

	if (res != 4) {
		printf("Write failed (%d)\n", res);
	}
	close(fich);
	return 0;
}
