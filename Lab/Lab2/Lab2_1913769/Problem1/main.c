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
#define SHM_KEY 0x123
#define SNAME "/semname"
#define MAPSIZE 1682
#define STRSIZE 40

struct Ratings
{
    int MovieID;
    float ratings;
    int count;
};

void getRating(const char *filename, struct Ratings* map, sem_t *sem)
{
    FILE *fptr = fopen(filename, "r");
    char str[STRSIZE];

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
        sem_wait(sem);
        if(map[arr[1] - 1].MovieID != arr[1]){
            map[arr[1]-1].MovieID = arr[1];
            map[arr[1]-1].count = 0;
            map[arr[1]-1].ratings = 0;
        }

        map[arr[1]-1].count++;
        map[arr[1]-1].ratings += arr[2];
        sem_post(sem);
    }
    fclose(fptr);
}

int main(int argc, char** argv) {
    int shmid;
    struct Ratings *map;

    sem_t *sem = sem_open(SNAME, O_CREAT, 0644);
    sem_init(sem, 0 ,1);

    shmid = shmget(SHM_KEY, MAPSIZE*sizeof(struct Ratings), 0644 | IPC_CREAT);
    if(shmid < 0) {
        perror("shared memory error");
        return 1;
    }
    else {
        printf("Shared memory: %d\n", shmid);
    }

    if(sem == SEM_FAILED) {
        printf("Sem failed\n");
        return -1;
    }

    map = (struct Ratings *) shmat(shmid, 0, 0);

    if(map == (struct Ratings*) -1) {
        perror("shmat");
        exit(1);
    }

    pid_t child1 = fork();
    pid_t child2 = -2;

    if (child1 == -1)
    {
        perror("fork 1");
    }

    if (child1 != 0)
    {
        child2 = fork();
        if (child2 == -1)
        {
            perror("fork 2");
        }
    }
    if(child1 == 0){
        getRating("movie-100k_1.txt", map, sem);
    }
    else if (child2 == 0) {
        getRating("movie-100k_2.txt", map, sem);
    }
    else{
        while(wait(NULL) > 0);
        FILE *rf = fopen("result.txt", "w+");

        for(int i = 0 ; i < MAPSIZE; i++) {
            float rating = map[i].ratings / map[i].count;
            fprintf(rf, "%d\t%f\n", i+1, rating);
        }

        fclose(rf);
    }

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
    // Remove the named semaphore
    if (sem_unlink(SNAME) < 0)
        perror("sem_unlink(3) failed");
    sem_close(sem);
    return 0;
}