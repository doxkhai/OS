#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "incircle.h"
#define NUM_THREADS 4

long eachThreads;
long innerPoints[NUM_THREADS];
int thread_count = NUM_THREADS;
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

    eachThreads = nPoints / NUM_THREADS; // calculate number of points each thread has to generate     // round up nPoints

    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_create(&threads[i], NULL, generatePoint, (void *)i);
        if (rc)
        {
            printf("ERROR; return from pthread create() is %d\n", rc);
            exit(-1);
        }
    }
    while(thread_count > 0);
    pthread_mutex_destroy(&mutex);

    long totalInnerpoints = 0;
    for (i = 0; i < NUM_THREADS; i++)
    {
        // rc = pthread_join(threads[i], NULL);
        // if (rc)
        // {
        //     printf("ERROR; return from pthread join() is %d\n", rc);
        //     exit(-1);
        // }
        totalInnerpoints += innerPoints[i];
    }

    double pi = 4 * totalInnerpoints / (double)nPoints;

    clock_t end = clock();
    double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Multi-thread:\nRun time: %f\n", runtime);
    printf("Total points: %ld\nIn circle Points: %ld\n", nPoints, totalInnerpoints);
    printf("PI: %f\n\n", pi);
    pthread_exit(NULL);
}

void *generatePoint(void *thread_arg)
{
    clock_t begin = clock();
    long idx = (long)thread_arg;
    for (long i = 0; i < eachThreads; i++)
    {
        if (inCircle(1, -1, 1))
        {
            innerPoints[idx]++;
        }
    }
    pthread_mutex_lock(&mutex);
    thread_count--;
    pthread_mutex_unlock(&mutex);
    clock_t end = clock();
    printf("Thread #%ld: %fs\n", idx, (double)(end-begin)/CLOCKS_PER_SEC);
    pthread_exit(NULL);
}
