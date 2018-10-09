#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>

#define DATASIZE	(1024*1024*200)

int data[DATASIZE];
struct timespec t1, t2;
long max_latency;

int main(int argc, char *argv[]){

	int i = 0;
	int loops;
	int fd;

	if(argc < 2){
		printf("Usage: %s loops\n", argv[0]);
		return -1;
	}

	loops = atoi(argv[1]);

	fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
	write(fd, "1", 1);
	while(loops--){
		long current_latency;
		
		clock_gettime(CLOCK_MONOTONIC, &t1);
		while(i < DATASIZE){
			data[i] = i;
			i++;
		}	
		clock_gettime(CLOCK_MONOTONIC, &t2);
		current_latency = ((long)(t2.tv_sec - t1.tv_sec) * 1000000) + (t2.tv_nsec / 1000) - (t1.tv_nsec / 1000);
		printf("Current: %ld\n", current_latency);
		i = 0;
	}
	write(fd, "0", 1);
	close(fd);
	return 0;
}
