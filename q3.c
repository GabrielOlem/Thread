#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 3
#define P 50
double x[NUM_THREADS][P];
double b[NUM_THREADS] = {7, -8, 6};
double a[NUM_THREADS][NUM_THREADS] = {10, 2, 1, 1, 5, 1, 2, 3, 10};
pthread_barrier_t barrier;
void *conta(void *threadid){
    int k = 0;
    int id = *(int*)threadid;
    while(k < P){
        int i = 0;
        double somatorio = 0;
        int j;
        for(j=0; j<NUM_THREADS; j++){
            if(j != id){
                somatorio += a[id][j]*x[j][k];
            }
        }
        x[id][k+1] = (1/a[id][id])*(b[id] - somatorio);
        k++;
        pthread_barrier_wait(&barrier);
    }
}

int main(int argc, char *argv[]){
    pthread_t thread[NUM_THREADS];
    int *ids[NUM_THREADS];
    int i;
    for(i=0; i<NUM_THREADS; i++){
        //x[i][0] =1;
    }
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    for(i=0; i<NUM_THREADS; i++){
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&thread[i], NULL, conta, (void*)ids[i]);
    }
    for(i = 0; i < NUM_THREADS; i++) { pthread_join(thread[i],NULL); }
    for(i=0; i<NUM_THREADS; i++){
        printf("%.2lf ", x[i][P-1]);
    }
    printf("\n");
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
}