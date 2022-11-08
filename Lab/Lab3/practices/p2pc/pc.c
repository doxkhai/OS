#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_ITEMS 4
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS 2 * MAX_ITEMS // variable

// Initiate shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

/*TODO: Fill in the synchronization stuff */
sem_t fullSem, emptySem, mutex ;
void put(int value); // put data into buffer
int get(); // get data from buffer

void * producer(void * arg) {
  int i;
  int tid = (int) arg;
  for (i = 0; i < LOOPS; i++) {
    /*TODO: Fill in the synchronization stuff */
    sem_wait(&emptySem);
    sem_wait(&mutex);
    put(i); // line P2
    sem_post(&mutex);
    printf("Producer %d put data %d\n", tid, i);
    sleep(1);
    sem_post(&fullSem);
    /*TODO: Fill in the synchronization stuff */
  }
  pthread_exit(NULL);
}

void * consumer(void * arg) {
  int i, tmp = 0;
  int tid = (int) arg;
  int semVal;
  while (tmp != -1) {
    /*TODO: Fill in the synchronization stuff */
    sem_wait(&fullSem);
    sem_wait(&mutex);
    tmp = get(); // line C2
    sem_post(&mutex);
    printf("Consumer %d get data %d\n", tid, tmp);
    sem_getvalue(&fullSem, &semVal);
    printf("fullSem: %d\n", semVal);
    sem_getvalue(&emptySem, &semVal);
    printf("emptySem: %d\n", semVal);
    sleep(1);
    sem_post(&emptySem);
    /*TODO: Fill in the synchronization stuff */
  }
  pthread_exit(NULL);
}

int main(int argc, char ** argv) {
  int i, j, semVal;
  int tid[THREADS];
  pthread_t producers[THREADS];
  pthread_t consumers[THREADS];

  /*TODO: Fill in the synchronization stuff */
  sem_init(&fullSem, PTHREAD_PROCESS_SHARED, 0);
  sem_init(&emptySem, PTHREAD_PROCESS_SHARED, MAX_ITEMS);
  sem_init(&mutex, PTHREAD_PROCESS_SHARED, 1);

  sem_getvalue(&fullSem, &semVal);
  printf("fullSem: %d\n", semVal);
  sem_getvalue(&emptySem, &semVal);
  printf("emptySem: %d\n", semVal);

  for (i = 0; i < THREADS; i++) {
    tid[i] = i;
    // Create producer thread
    pthread_create( & producers[i], NULL, producer, (void * ) tid[i]);

    // Create consumer thread
    pthread_create( & consumers[i], NULL, consumer, (void * ) tid[i]);
  }

  for (i = 0; i < THREADS; i++) {
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
  }

  /*TODO: Fill in the synchronization stuff */
  sem_close(&fullSem);
  sem_close(&emptySem);
  sem_close(&mutex);
  return 0;
}

void put(int value) {
  buffer[fill] = value; // line f1
  fill = (fill + 1) % MAX_ITEMS; // line f2
}

int get() {
  int tmp = buffer[use]; // line g1
  // buffer[use] = -1;
  use = (use + 1) % MAX_ITEMS; // line g2
  return tmp;
}