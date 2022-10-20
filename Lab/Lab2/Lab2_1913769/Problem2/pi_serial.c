#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "incircle.h"

int main(int argc, char** argv) {
    clock_t begin = clock();
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

    long innerPoints = 0;
    for(long i = 0; i < nPoints; i++)
        if(inCircle(1,-1,1))
            innerPoints++;

    double pi = 4 * innerPoints / (double)nPoints;

    clock_t end = clock();
    double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
   
    FILE *fptr = fopen("runtime.txt", "a");
    fprintf(fptr, "Points: %ld\n Serial: %f - %f\n", nPoints, pi, runtime);
    fclose(fptr);

    return 0;
}