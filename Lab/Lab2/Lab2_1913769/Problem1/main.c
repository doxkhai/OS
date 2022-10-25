#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#define SHM_KEY 0x123
#define SNAME "/semname"
#define MAPSIZE 1682
#define STRSIZE 40
#define FILE1 "movie-100k_1.txt"
#define FILE2 "movie-100k_2.txt"

struct Ratings
{
    int MovieID;
    float ratings;
    int count;
};

int main(int argc, char **argv)
{

    int shmid;
    struct Ratings *map;

    sem_t *sem = sem_open(SNAME, O_CREAT, 0644,1);

    if (sem == SEM_FAILED)
    {
        printf("Sem failed\n");
        return -1;
    }

    if (sem_init(sem, 1, 1) != 0) {
        printf("Sem init failed\n");
        return -1;
    }

    shmid = shmget(SHM_KEY, (MAPSIZE + 1) * sizeof(struct Ratings), 0644 | IPC_CREAT);
    if (shmid < 0)
    {
        printf("Shared memory error");
        return -1;
    }

    pid_t child1 = fork();
    pid_t child2 = -2;

    if (child1 == -1)
    {
        printf("fork 1");
    }

    if (child1 != 0)
    {
        child2 = fork();
        if (child2 == -1)
        {
            printf("fork 2");
        }
    }

    map = (struct Ratings *)shmat(shmid, 0, 0);

    if (map == (struct Ratings *) -1)
    {
        perror("shmat");
        exit(1);
    }

    if (child1 == 0 || child2 == 0)
    {
        char* filename  = child1 == 0 ? FILE1 : FILE2;
        
        FILE *fptr = fopen(filename, "r");
        char str[STRSIZE];
        int count = 0;

        while (fgets(str, STRSIZE, fptr))
        {
            int arr[4];
            int i = 0;
            str[strlen(str) - 1] = '\0';

            char *token = strtok(str, " \t");

            while (token != NULL)
            {
                arr[i] = atoi(token);
                token = strtok(NULL, " \t");
                i++;
            }
            if (sem_wait(sem) < 0)
                printf("sem_wait(3) failed\n");
    
            if (map[arr[1] - 1].MovieID != arr[1])
            {
                map[arr[1] - 1].MovieID = arr[1];
                map[arr[1] - 1].count = 0;
                map[arr[1] - 1].ratings = 0;
            }

            map[arr[1] - 1].count++;
            map[arr[1] - 1].ratings += arr[2];
            if(sem_post(sem) < 0)
                printf("sem_post(3) failed\n");
            memset(str, 0, strlen(str));
        }
        fclose(fptr);
        if (shmdt(map) == -1)
        {
            perror("shmdt");
            return 1;
        }
    }
    else if (child1 != 0 && child2 != 0)
    {
        while (wait(NULL) > 0);
        // Remove the named semaphore
        sem_close(sem);
        if (sem_unlink(SNAME) < 0)
            perror("sem_unlink(3) failed");

        FILE *rf = fopen("finaloutput.txt", "w+");

        for (int i = 0; i < MAPSIZE; i++)
        {
            if (map[i].MovieID != i+1)
                continue;
            float rating = map[i].ratings / map[i].count;
            fprintf(rf, "%d\t%f\n", map[i].MovieID, rating);
        }

        fclose(rf);
        if (shmdt(map) == -1)
        {
            perror("shmdt");
            return 1;
        }
        if (shmctl(shmid, IPC_RMID, 0) == -1)
        {
            perror("shmctl");
            return 1;
        }
    }
    

    return 0;
}
