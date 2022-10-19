#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum; //this data is shared by the thread(s)
void* runner(void* param); // threads call this function

int main(int argc, char *argv[]){
    pthread_t tid; //thread id
    pthread_attr_t attr;  // set of thread's attributes

    if(argc != 2) {
        fprintf(stderr, "usage: a.out 1\n");
        return -1;
    }

    if(atoi(argv[1]) < 0) {
        fprintf(stderr, "%d must be >=0 \n", atoi(argv[1]));
        return -1;
    }

    //get the default attributes
    pthread_attr_init(&attr);
    
    //create the thread
    pthread_create(&tid, &attr, runner, argv[1]);
    printf("Thread id: %ld\n", (long)tid);

    //wait for the thread to exit
    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
}

//the thread will begin control in this func
void* runner(void * param) {
    int i, upper = atoi(param);
    sum = 0;
    for(i = 1; i <= upper; i++)
        sum += i;
    sleep(1);
    pthread_exit(0);
}
