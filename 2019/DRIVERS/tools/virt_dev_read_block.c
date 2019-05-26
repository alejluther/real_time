#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{

	int fich, res;
	int i, data, count;
	char filename[20];

	if (argc < 3) {
		printf("Usage: %s device count\n", argv[0]);
		return -1;
	}

	count = atoi(argv[2]);

	memset(&filename, '\0', 20);
	sprintf(filename, "%s", argv[1]);

	fich = open(filename, O_RDWR);
	if (fich < 0) {
		printf("Opening file %s failed\n", filename);
		return -1;
	}

	for (i = 0; i < count; i++) {
		res = read(fich, &data, 4);
		if (res < 0)
			printf("Error\n");
		if (res == 0)
			printf("No data\n");
		if (res > 0)
			printf("Reading: %d\n", data);
	}

	close(fich);
	return 0;
}
