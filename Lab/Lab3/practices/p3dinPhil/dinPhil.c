#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

pthread_mutex_t mtx;
pthread_cond_t chopstick[N];
int trigger = 1;

void *philosopher(void *);
void eat(int);
void think(int);

int main()
{
   int i, a[N];
   pthread_t tid[N];

   // BEGIN PROTECTION MECHANISM
   pthread_mutex_init(&mtx, NULL);

   for (i = 0; i < N; i++)
      pthread_cond_init(&chopstick[i], NULL);

   // END PROTECTION MECHANISM
   for (i = 0; i < 5; i++)
   {
      a[i] = i;
      pthread_create(&tid[i], NULL, philosopher, (void *)&a[i]);
   }
   sleep(2);
   trigger = 0;
   while (1)
   {
      pthread_cond_signal(&chopstick[0]);
      pthread_cond_signal(&chopstick[(0 + 1) % N]);
   }

   for (i = 0; i < 5; i++)
      pthread_join(tid[i], NULL);
}

void *philosopher(void *num)
{
   int phil = *(int *)num;
   printf("Philosopher %d has entered room\n", phil);

   while (1)
   {
      // PROTECTION MECHANISM
      pthread_mutex_lock(&mtx);
      pthread_cond_wait(&chopstick[phil], &mtx);
      pthread_cond_wait(&chopstick[(phil + 1) % N], &mtx);

      printf("Philosopher %d takes fork %d and %d\n",
             phil, phil, (phil + 1) % N);

      eat(phil);
      sleep(2);

      printf("Philosopher %d puts fork %d and %d down\n",
             phil, phil, (phil + 1) % N);
      // PROTECION MECHANISM
      pthread_mutex_unlock(&mtx);
      for (int i = 0; i < N; i++)
      {
         pthread_cond_signal(&chopstick[phil + 1]);
         pthread_cond_signal(&chopstick[(phil + 2) % N]);
      }

      think(phil);
      sleep(1);
   }
}

void eat(int phil)
{
   printf("Philosopher %d is eating\n", phil);
}

void think(int phil)
{
   printf("Philosopher %d is thinking\n", phil);
}