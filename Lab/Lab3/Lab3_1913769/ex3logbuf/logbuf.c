#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];

int count = 0;
pthread_mutex_t mtx;
pthread_cond_t cond;
void flushlog();

struct _args
{
   unsigned int interval;
};

void *wrlog(void *data)
{
   char str[MAX_LOG_LENGTH];
   int id = *(int *)data;

   pthread_mutex_lock(&mtx);
   while (count >= MAX_BUFFER_SLOT)
      pthread_cond_wait(&cond, &mtx);

   usleep(20);
   sprintf(str, "%d", id);
   strcpy(logbuf[count], str);
   /* Only increase count to size MAX_BUFFER_SLOT - 1*/
   count++;
   // printf("write log, thread: %d, count: %d\n", id, count);
   pthread_mutex_unlock(&mtx);

   return 0;
}

void flushlog()
{
   int i;
   char nullval[MAX_LOG_LENGTH];

   pthread_mutex_lock(&mtx);
   // printf("flushlog() %d\n", count);
   sprintf(nullval, "%d", -1);
   for (i = 0; i < count; i++)
   {
      printf("Slot  %i: %s\n", i, logbuf[i]);
      strcpy(logbuf[i], nullval);
   }
   /*Reset buffer */
   count = 0;
   pthread_mutex_unlock(&mtx);
   pthread_cond_broadcast(&cond);
   fflush(stdout);
   return;
}

void *timer_start(void *args)
{
   while (1)
   {
      flushlog();
      /*Waiting until the next timeout */
      usleep(((struct _args *)args)->interval);
   }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;
   int id[MAX_LOOPS];
   pthread_mutex_init(&mtx, NULL);
   pthread_cond_init(&cond, NULL);

   struct _args args;
   /*500 msec ~ 500 * 1000 usec */
   args.interval = 500e3;

   /*Setup periodically invoke flushlog() */
   pthread_create(&lgrid, NULL, &timer_start, (void *)&args);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void *)&id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   sleep(5);

   return 0;
}
