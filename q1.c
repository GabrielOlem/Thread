#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int contador = 0;
int acabou1 = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void *conta(void *threadid){
    long tid = *((long *)threadid);
    while(1){
        
        pthread_mutex_lock(&mymutex);
        if(contador == 1000000){
            if(acabou1 == 1){
                pthread_mutex_unlock(&mymutex);
                pthread_exit(NULL);
            }
            acabou1 = 1;
            printf("terminei %i %li\n", contador, tid);
            pthread_mutex_unlock(&mymutex);
            pthread_exit(NULL);
        }
        contador++;
        pthread_mutex_unlock(&mymutex);
    }
}
int main(int argc, char *argv[]){
    int n;
    scanf("%i", &n);
    int i;
    pthread_t thread[n];
    long *taskids[n];

    for(i=0; i<n; i++){
        taskids[i] = (long*)malloc(sizeof(long));
        *taskids[i] = i;
        int rc = pthread_create(&thread[i], NULL, conta, (void *)taskids[i]);
        if(rc){
            printf("deu merda\n");
            return -1;
        }
        if(contador == 100){
            pthread_exit(NULL);
        }
    }
    for(i=0; i<n; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_exit(NULL);
}