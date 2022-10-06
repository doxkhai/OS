#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	printf("Process Id: %d\n", getpid());
	printf("Parent process ID: %d\n", getppid()) ;
	printf("My group: %d\n", getpgrp());
	return 0;
}
