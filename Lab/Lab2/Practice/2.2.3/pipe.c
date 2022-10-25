#include <stdio.h>
#include <unistd.h>

int main()
{
    int pipefds[2];
    int returnstatus;
    int pid;
    char writemessages[20] = "Hello";
    char writemessages1[20] = "World";
    char readmessage[20];
    returnstatus = pipe(pipefds);
    if (returnstatus == -1)
    {
        printf("Unable to create pipe\n");
        return 1;
    }
    pid = fork();

    // Child process
    if (pid == 0)
    {
        // sleep(5);
        read(pipefds[0], readmessage, sizeof(readmessage));
        printf("child process: reading, message is : %s\n", readmessage);
    }

    // Parent process
    else
    {
        sleep(3);
        printf("Parent process: writing message is %s\n", writemessages);
        write(pipefds[1], writemessages, sizeof(writemessages));
        read(pipefds[0], readmessage, sizeof(readmessage));
        printf("Parent process: reading, message is : %s\n", readmessage);
    }
    return 0;
}