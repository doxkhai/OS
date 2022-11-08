#ifndef PTHREAD_H
#include <pthread.h>
#endif

/*
   - 1 writer at a time
   - when writer enter CS, seq number += 1 to an odd value. when exit CS, seq number -= 1 to an even value
   - if reader wants to enter, check if seq number is odd value, if true then wait for writer to finish
   - when value is even, many readers can enter critical section.
   - if there's only 1 reader and 0 writer in CS, writer can enter CS
*/

typedef struct pthread_seqlock
{ /* TODO: implement the structure */
   pthread_mutex_t mtx;
   int seqCount;
   int readerCount;
} pthread_seqlock_t;

static inline int pthread_seqlock_init(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */
   seqlock->seqCount = 0;
   seqlock->readerCount = 0;
   if(pthread_mutex_init(&(seqlock->mtx), NULL)) 
      return 0;
   return 1;
}

static inline int pthread_seqlock_destroy(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */
   if(pthread_mutex_destroy(&(seqlock->mtx))) 
      return 0;
   free(seqlock);
   return 1;
}

static inline int pthread_seqlock_wrlock(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */
   while (1)
   {
      pthread_mutex_lock(&(seqlock->mtx));
      if (seqlock->seqCount % 2 == 0 && seqlock->readerCount < 2)
         break;
      else
         pthread_mutex_unlock(&(seqlock->mtx));
   }
   seqlock->seqCount++;
   pthread_mutex_unlock(&(seqlock->mtx));
   return 1;
}

static inline int pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */
   pthread_mutex_lock(&(seqlock->mtx));
   if(seqlock->seqCount == 1)
      seqlock->seqCount--;
   else return 0;
   pthread_mutex_unlock(&(seqlock->mtx));
   return 1;
}

static inline int pthread_seqlock_rdlock(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */

   while (1)
   {
      pthread_mutex_lock(&(seqlock->mtx));
      if (seqlock->seqCount % 2 == 0)
         break;
      else
         pthread_mutex_unlock(&(seqlock->mtx));
   }
   seqlock->readerCount++;
   pthread_mutex_unlock(&(seqlock->mtx));
   return 1;
}

static inline int pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock)
{
   /* TODO: ... */
   pthread_mutex_lock(&(seqlock->mtx));
   if (seqlock->readerCount > 0)
      seqlock->readerCount--;
   else
      return 0;
   pthread_mutex_unlock(&(seqlock->mtx));
   return 1;
}