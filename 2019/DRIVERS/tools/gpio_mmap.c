#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <inttypes.h>

int
main(int argc, char *argv[])
{

	int fich;
	void *gpio_base;
    uint32_t reg_val;

	fich = open("/dev/gpio_mmap_0", O_RDWR);
	if (fich < 0){
		printf("Fallo al abrir /dev/gpio_mmap_0\n");
		return -1;
	}

	gpio_base = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fich, 0);
	if (gpio_base < 0) {
		printf("Error when mmaping\n");
		return -1;
	}

    printf("mmaping at %p\n", gpio_base);

    memcpy(&reg_val, gpio_base, 4);
    printf("REG: %08x\n", reg_val);

	gpio_base += 4;
    memcpy(&reg_val, gpio_base, 4);
    printf("REG: %08x\n", reg_val);

	gpio_base += 4;
    memcpy(&reg_val, gpio_base, 4);
    printf("REG: %08x\n", reg_val);

    munmap(gpio_base, 4096);
    close(fich);

    return 0;
}
