#include <stdio.h>
#include <semaphore.h>
main()
{
    sem_t my_semaphore;
    int value;

    sem_init(&my_semaphore, 0, 10);
    sem_getvalue(&my_semaphore, &value);
    printf("The initial value of the semaphore is %d\n", value);
    sem_wait(&my_semaphore);
    sem_getvalue(&my_semaphore, &value);
    printf("The value of the semaphore after the wait is %d\n", value);
}