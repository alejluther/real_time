#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	cpu_set_t set;
	int res;
	int cpu_to_set;
	int pid_to_set;

	cpu_to_set = atoi(argv[2]);
	pid_to_set = atoi(argv[1]);
	CPU_ZERO(&set);
	printf("Setting cpu %d to pid %d\n", cpu_to_set, pid_to_set);
	CPU_SET(cpu_to_set, &set);
	res = sched_setaffinity((pid_t)pid_to_set, sizeof(set), &set);
	if (res < 0) {
		printf("Error with sched_setaffinity: %d\n",errno);
		return -1;
	}
	
}
