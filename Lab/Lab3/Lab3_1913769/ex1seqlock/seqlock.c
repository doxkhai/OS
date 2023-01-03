#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "seqlock.h" /* TODO implement this header file */

pthread_seqlock_t lock;

int main()
{
   int val = 0;

   if (pthread_seqlock_init(&lock) == 0)
   {
      printf("Error init\n");
      return -1;
   };

   // pthread_seqlock_rdlock(&lock);
   pthread_seqlock_rdlock(&lock);
   pthread_seqlock_wrlock(&lock);
   val++;
   printf("%d\n", val);
   
   if (pthread_seqlock_rdunlock(&lock) == 0)
   {
      printf("Error read unlock\n");
      return -1;
   }

   if (pthread_seqlock_wrunlock(&lock) == 0)
   {
      printf("Error write unlock\n");
      return -1;
   };
   

   if (pthread_seqlock_rdlock(&lock) == 1)
   {
      printf("val = %d\n", val);
      if (pthread_seqlock_rdunlock(&lock) == 0)
      {
         printf("Error read unlock\n");
         return -1;
      }
   }
}
