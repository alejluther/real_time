#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define MAX_CLIENTS 10

int fich[MAX_CLIENTS];
int dev_fd;
int clients_total;

int client_order;

pthread_mutex_t main_lock;

static void *
client_thread(void *arg) {
	int id;
	int val;

	pthread_mutex_lock(&main_lock);
	id = client_order++;
	pthread_mutex_unlock(&main_lock);

	printf("Dentro de thread: %d\n", id);

	while (read(fich[id], &val, 4)) {
		if (id % 2)
			write(dev_fd, &val, val);
		else
			read(dev_fd, &val, val);
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]) {

	char filename[30];
	pthread_t thread_id[MAX_CLIENTS];
	int i;

	if (argc != 2) {
		printf("Usage: %s clients_total\n", argv[0]);
		return -1;
	}

	clients_total = atoi(argv[1]);

	for (i = 0; i < clients_total; i++) {
		memset(filename, 0, 30);
		sprintf(filename, "data_%d", i);
		fich[i] = open(filename, O_RDWR);
		if (fich[i] < 0) {
			printf("Error when creating file %s\n", filename);
			return -EIO;
		}
	}

	dev_fd = open("/dev/virt_dev_bank", O_RDWR);
	if (dev_fd < 0) {
		printf("Error opening file /dev/virt_dev_bank\n");
		return -EINVAL;
	}

	for (i = 0; i < clients_total; i++) {
		if (pthread_create(&thread_id[i], 0, &client_thread, &i) != 0) {
			printf("error in pthread_create for thread %d (errno: %d)\n", i, errno);
            return -ENOMEM;
		}
	}

	for (i = 0; i < clients_total; i++) {
		pthread_join(thread_id[i], 0);
		close(fich[i]);
	}
}
