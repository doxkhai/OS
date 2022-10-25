#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSG_SIZE 20

static int pipefd1[2], pipefd2[2];
// pipefd1: child write, parent read
// pipefd2: parent write, child read

void INIT(void)
{
    if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void WRITE_TO_PARENT(void)
{
    /* send parent a message through pipe */
    // TODO
    char msg[MSG_SIZE] = "Hello dad <3";
    printf("Child process: Writing, message is: %s\n", msg);
    write(pipefd1[1], msg, sizeof(msg));
    printf("Child send message to parent!\n\n");
}

void READ_FROM_PARENT(void)
{
    /* read message sent by parent from pipe */
    char msg[MSG_SIZE];
    // TODO
    read(pipefd2[0], msg, sizeof(msg));
    printf("Child receive message from parent: %s\n\n", msg);
}

void WRITE_TO_CHILD(void)
{
    /* send child a message through pipe */
    // TODO
    char msg[MSG_SIZE] = "Hi son!";
    printf("Parent process: Writing, message is: %s\n", msg);
    write(pipefd2[1], msg, sizeof(msg));
    printf("Parent send message to child!\n\n");
}

void READ_FROM_CHILD(void)
{
    /* read message sent by child from pipe */
    char msg[MSG_SIZE];
    // TODO
    read(pipefd1[0], msg, sizeof(msg));
    printf("Parent receive message from child: %s\n\n", msg);
}

int main(int argc, char **argv)
{
    INIT();
    pid_t pid;
    pid = fork();

    // set a timer, process will end after 1 second.
    alarm(10);
    if (pid != 0)
    {
        // Parent process
        while (1)
        {
            sleep(rand() % 2 + 1);
            WRITE_TO_CHILD();
            READ_FROM_CHILD();
        }
    }
    else
    {
        // Child process
        while (1)
        {
            sleep(rand() % 2 + 1);
            WRITE_TO_PARENT();
            READ_FROM_PARENT();
        }
    }

    return 0;
}