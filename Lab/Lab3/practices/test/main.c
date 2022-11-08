#include "main.h"
#include <stdio.h>


int main() {
    int arr[] = {1,2,3,4,5};
    int n = sizeof(arr)/sizeof(int);

    printf("sum: %d\n", sumArray(arr,n));
    return 0;
}