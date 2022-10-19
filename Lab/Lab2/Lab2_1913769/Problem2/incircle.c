#include "incircle.h"
#include <stdlib.h>

double genRandom(int upper, int lower)
{
    return ((double)rand() / (double)RAND_MAX) * (upper - lower) + lower;
}

int inCircle(double upper, double lower, double r)
{
    double x = genRandom(upper, lower),
           y = genRandom(upper, lower);

    return sqrt(x*x + y*y) < r ? 1 : 0;
}