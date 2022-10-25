#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "gettime.h"

struct thread_arg
{
    long long eachThreads;
    unsigned int rand_state;
    long long innerPoints;
};

void *generatePoint(void *thread_arg);

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "usage: ./pi_multi-thread <number of points>\n");
        return -1;
    }

    long long nPoints = atoll(argv[1]);

    if (nPoints < 1)
    {
        fprintf(stderr, "%d must be >=1 \n", atoi(argv[1]));
        return -1;
    }

    long numThreads;
    if (nPoints >= 1000000000)
        numThreads = 1000;
    else if (nPoints >= 100000000)
        numThreads = 100;
    else if (numThreads >= 10000000)
        numThreads = 10;
    else
        numThreads = 1;


    //Start timer
    struct timespec start, finish, delta;
    clock_gettime(CLOCK_REALTIME, &start);

    // calculate number of points each thread has to generate
    long long eachThreads = nPoints / numThreads;
    long long lastThreads = nPoints % numThreads;
    long long totalInnerpoints = 0;

    pthread_t threads[numThreads];
    struct thread_arg thread_args[numThreads];

    int rc;
    long i;
    for (i = 0; i < numThreads; i++)
    {
        thread_args[i].eachThreads = eachThreads;
        thread_args[i].rand_state = rand();
        if(i == numThreads - 1 && i != 0)
            thread_args[i].eachThreads += lastThreads; 
        rc = pthread_create(&threads[i], NULL, generatePoint, (void *)&thread_args[i]);
        if (rc)
        {
            printf("ERROR; return from pthread create() is %d\n", rc);
            exit(-1);
        }
    }

    for (i = 0; i < numThreads; i++)
    {
        rc = pthread_join(threads[i], NULL);
        if (rc)
        {
            printf("ERROR; return from pthread join() is %d\n", rc);
            exit(-1);
        }
        totalInnerpoints += thread_args[i].innerPoints;
    }

    double pi = 4 * totalInnerpoints / (double)nPoints;

    //End timer
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);

    FILE *fptr = fopen("runtime.txt", "a");
    fprintf(fptr, "Points: %lld\n Multi-thread, %ld threads: %f - %d.%lds\n", nPoints, numThreads, pi, (int)delta.tv_sec, delta.tv_nsec/10000);
    fclose(fptr);

    pthread_exit(NULL);
}

void *generatePoint(void *thread_arg)
{

    struct thread_arg *data = (struct thread_arg *)thread_arg;
    long long innerPoints = 0;
    long long each = data->eachThreads;
    unsigned int rand_state = data->rand_state;
    long long i;
    double x, y;
    for (i = 0; i < each; i++)
    {
        x = rand_r(&rand_state) / ((double)RAND_MAX) * 2.0 - 1.0;
        y = rand_r(&rand_state) / ((double)RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y < 1)
        {
            innerPoints++;
        }
    }
    data->innerPoints = innerPoints;
    pthread_exit(NULL);
}
