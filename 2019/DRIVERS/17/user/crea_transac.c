#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fich;
	int clients_total;
	int transac_total;
	int initial_deposit;
	char filename[30];
	int i, j, val;

	if (argc != 3) {
		printf("Usage: %s clients_total transac_total\n", argv[0]);
		return -1;
	}

	initial_deposit = 10000000;

	clients_total = atoi(argv[1]);
	transac_total = atoi(argv[2]);

	for (i = 0; i < clients_total; i++) {
		memset(filename, 0, 30);
		sprintf(filename, "data_%d", i);
		fich = open(filename, O_CREAT | O_RDWR);
		if (fich < 0) {
			printf("Error when creating file %s\n", filename);
			return -EIO;
		}
		for (j = 0; j < transac_total; j++) {
			val = rand() & 0x3ff;
			if (write(fich, &val, 4) != 4) {
				printf("write error with file %s (%d)\n", filename, j);
				return -EIO;
			}
			if (i % 2)
				initial_deposit += val;
			else
				initial_deposit -= val;
		}
		printf("Deposit after %d: %d\n", i, initial_deposit);
		close(fich);
	}
}
