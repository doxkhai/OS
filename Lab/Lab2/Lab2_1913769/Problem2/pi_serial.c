#include <stdlib.h>
#include <stdio.h>
#include "gettime.h"

int main(int argc, char** argv) {
    if (argc != 2)
    {
        fprintf(stderr, "2 argc required!\n");
        return -1;
    }

    long long nPoints = atoll(argv[1]);

    if (nPoints < 1)
    {
        fprintf(stderr, "%d must be >=1 \n", atoi(argv[1]));
        return -1;
    }

    struct timespec start, finish, delta;
    clock_gettime(CLOCK_REALTIME, &start);
 
    long long innerPoints = 0;
    unsigned int rand_state = rand();
    long long i;
    double x,y;
    for (i = 0; i < nPoints; i++)
    {
        x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

        if (x * x + y * y < 1)
        {
            innerPoints++;
        }
    }

    double pi = 4 * innerPoints / (double)nPoints;

    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
   
    FILE *fptr = fopen("runtime.txt", "a");
    fprintf(fptr, "Points: %lld\n Serial: %f - %d.%lds\n", nPoints, pi, (int)delta.tv_sec, delta.tv_nsec/10000);
    fclose(fptr);

    return 0;
}