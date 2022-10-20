#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "incircle.h"
#define NUM_THREADS 1

long totalInnerpoints = 0;
pthread_mutex_t mutex;

void *generatePoint(void *thread_arg);

int main(int argc, char **argv)
{
    clock_t begin = clock();
    pthread_t threads[NUM_THREADS];

    int rc;
    long i;

    if (argc != 2)
    {
        fprintf(stderr, "2 argc required!\n");
        return -1;
    }

    long nPoints = atoi(argv[1]);

    if (nPoints < 1)
    {
        fprintf(stderr, "%d must be >=1 \n", atoi(argv[1]));
        return -1;
    }

    // calculate number of points each thread has to generate
    long eachThreads = nPoints / NUM_THREADS;

    // pass to an array which value of each element is set equally, but with different addresses
    long each[NUM_THREADS];
    
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < NUM_THREADS; i++)
    {
        each[i] = eachThreads;
        rc = pthread_create(&threads[i], NULL, generatePoint, (void *)each[i]);
        if (rc)
        {
            printf("ERROR; return from pthread create() is %d\n", rc);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i], NULL);
        if (rc)
        {
            printf("ERROR; return from pthread join() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_mutex_destroy(&mutex);

    double pi = 4 * totalInnerpoints / (double)nPoints;

    clock_t end = clock();
    double runtime = (double)(end - begin) / CLOCKS_PER_SEC;

    FILE *fptr = fopen("runtime.txt", "a");
    fprintf(fptr, "Points: %ld\n Multi-thread: %f - %f\n", nPoints, pi, runtime);
    fclose(fptr);

    pthread_exit(NULL);
}

void *generatePoint(void *thread_arg)
{
    long each = (long)thread_arg;
    long innerPoints = 0;
    for (long i = 0; i < each; i++)
        if (inCircle(1, -1, 1))
            innerPoints++;

    pthread_mutex_lock(&mutex);
    totalInnerpoints += innerPoints;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
