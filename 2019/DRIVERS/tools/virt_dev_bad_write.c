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

	int fich, minor;
	char filename[20];
	char *doctor_maligno;

	if (argc != 4) {
		printf("Usage: %s device minor numero\n", argv[0]);
		return -1;
	}

	minor = atoi(argv[2]);

	memset(filename, '\0', 20);
	sprintf(filename, "%s%d", argv[1], minor);

	fich = open(filename, O_RDWR);

	if (fich < 0) {
		printf("Fallo al abrir device %s\n", filename);
		return -1;
	}

	doctor_maligno = (char *)-1;
	write(fich, doctor_maligno, 4);
	close(fich);
	return 0;
}
