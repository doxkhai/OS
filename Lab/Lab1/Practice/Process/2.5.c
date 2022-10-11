#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // defines fork(), and pid_t. 

int main(int argc, char** argv) {
	
	pid_t child_pid;

	//lets fork off a child process...
	child_pid = fork();
	
	// check what the fork() call actually did	
	if(child_pid == -1){
		perror("fork");
		exit(1);
	}

	if(child_pid == 0){
		//fork() succeeded, we're indside the child process
		printf("hello ");
		fflush(stdout);
	}
	else {
		//fork() succeeded, we're inside the parent process
		wait(NULL);
		printf("world!\n");
		fflush(stdout);
	}
		

	return 0;
}
