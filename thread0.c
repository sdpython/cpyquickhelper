// thread0.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker_thread(void *arg)
{
        printf("This is worker_thread()\n");
        pthread_exit(NULL);
}

int main()
{
        pthread_t my_thread;
        int ret;

        printf("In main: creating thread\n");
        ret =  pthread_create(&my;_thread, NULL, &worker;_thread, NULL);
        if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
        }

        pthread_exit(NULL);
}