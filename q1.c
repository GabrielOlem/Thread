#include <pthread.h>
#include <stlib.h>
#include <stdio.h>

int contador = 0;
void *conta(void *threadid){
    long lid = *((long *) threadid);
    if(contador == 1000000){
        printf("Terminei %i %ld\n", contador, lid);
        pthread_exit(NULL);
    }
}