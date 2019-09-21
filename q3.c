#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_VARIAVEIS 3
#define P 50
double x[NUM_VARIAVEIS][P];
double b[NUM_VARIAVEIS] = {7, -8, 6};
double a[NUM_VARIAVEIS][NUM_VARIAVEIS] = {10, 2, 1, 1, 5, 1, 2, 3, 10};
int **respo;
int NUM_THREADS;
pthread_barrier_t barrier;

void *conta(void *threadid){
    int k = 0;
    int id = *(int*)threadid;
    while(k < P){
        int contador = 0;
        int novoId = respo[id][contador];
        while(1){
            double somatorio = 0;
            int j;
            for(j=0; j<NUM_VARIAVEIS; j++){
                if(j != novoId){
                    somatorio += a[novoId][j]*x[j][k];
                }
            }
            x[novoId][k+1] = (1/a[novoId][novoId])*(b[novoId] - somatorio);
            
            contador++;
            if(contador > NUM_VARIAVEIS){
                break;
            }
            novoId = respo[id][contador];
            if(novoId == -1){
                break;
            }
        }
        k++;
        pthread_barrier_wait(&barrier);
    }
}

int main(int argc, char *argv[]){
    scanf("%i", &NUM_THREADS);
    if(NUM_THREADS > NUM_VARIAVEIS){
        NUM_THREADS = NUM_VARIAVEIS;
    }
    respo = (int **) malloc(sizeof(int*)*NUM_THREADS);
    int c;
    for(c=0; c<NUM_THREADS; c++){
        respo[c] = (int*)malloc(sizeof(int)*NUM_VARIAVEIS);
        int d = 0;
        for(d=0; d<NUM_VARIAVEIS; d++){
            respo[c][d] = -1;
        }
    }
    int a;
    int contad = 0;
    int filtro = 0;
    for(a=0; a<NUM_VARIAVEIS; a++){
        contad = a % NUM_THREADS;
        filtro = floor(a / NUM_THREADS);
        respo[contad][filtro] = a;
    }
    pthread_t thread[NUM_THREADS];
    int *ids[NUM_THREADS];
    int i;
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    for(i=0; i<NUM_THREADS; i++){
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&thread[i], NULL, conta, (void*)ids[i]);
        
    }
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i],NULL);
    }
    for(i=0; i<NUM_VARIAVEIS; i++){
        printf("%.2lf ", x[i][P-1]);
    }
    printf("\n");
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
}