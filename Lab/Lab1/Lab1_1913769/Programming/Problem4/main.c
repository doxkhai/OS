#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXSZ 100

void countNumber(int arr[], int n, int div){
    int count = 0;
    for(int i = 0 ; i < n ; i++) {
        if(arr[i] % div == 0) count++;
    }
    printf("%d\n", count);
}

int main(int argc, char** argv){

    FILE* fptr = fopen("numbers.txt", "r");
    int integers[MAXSZ];

    int n = 0;
    int num;
    
    while(fscanf(fptr, "%d", &num) == 1 && n < MAXSZ){
        integers[n] = num;
        n++;
    }

    pid_t pid = fork();

    if(pid == -1){
        perror("fork");
    }

    countNumber(integers, n, pid == 0 ? 3 : 2);
    fclose(fptr);
    return 0;
}