#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 5

struct thread_data
{
    int thread_id;
    int sum;
    char *message;
};
int sum = 0;

struct thread_data thread_data_array[NUM_THREADS];

void *printHello(void *thread_arg)
{
    struct thread_data *my_data = (struct thread_data *)thread_arg;

    printf("chung: %d\n", sum++);
    printf("Hello world! It's me thread #%d!\n", my_data->thread_id);
    printf("Sum #%d: %d\n", my_data->thread_id, my_data->sum);
    printf("Message #%d: %s\n", my_data->thread_id, my_data->message);
    sleep(1);

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("In main: creating thread %ld\n", t);
        thread_data_array[t].thread_id = t;
        thread_data_array[t].sum = t * 2;
        thread_data_array[t].message = "hello\n";
        rc = pthread_create(&threads[t], NULL, printHello, (void *)&thread_data_array[t]);
        if (rc)
        {
            printf("ERROR; return from pthread create() is %d\n", rc);
            exit(-1);
        }
    }
    
    printf("HAHAHAHHAHAHAHAHA\n");
    pthread_exit(NULL);
}