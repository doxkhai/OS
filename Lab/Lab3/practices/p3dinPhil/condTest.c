#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#define NUM 5

pthread_mutex_t mtx;
pthread_cond_t cond[NUM];
pthread_cond_t con;

int state = 1;

void* test(void* arg);

int main() {
    pthread_t threads[NUM];
    pthread_mutex_init(&mtx,NULL);
    int i, a[NUM];

    pthread_cond_init(&con, NULL);

    for(i = 0; i < NUM; i++){
        a[i] = i;
        pthread_cond_init(&cond[i],NULL);
        pthread_create(&threads[i], NULL, test, (void*) &a[i]);
    }


    // while(1)
    sleep(2);
    state = 0;
    pthread_cond_signal(&con);
    for(i = 0; i < NUM; i++){
        pthread_join(threads[i], NULL);
    }
}

void* test(void* arg) {
    int i = *(int*)arg;
    
    // pthread_mutex_lock(&mtx);
    while(state)
        pthread_cond_wait(&con, &mtx);
    printf("Hello %d\n", i);
    sleep(1);
    // pthread_cond_signal(&cond[(i+1)]);
    // pthread_mutex_unlock(&mtx);
    printf("%d successful\n", i);
    return NULL;
}

