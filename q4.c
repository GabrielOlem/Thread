#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int primos = 0;
pthread_mutex_t mutexPrimos;
pthread_mutex_t mutexDivisores;
int divisores = 0;
int nmaior = 0;

typedef struct test{
    int a, b;
}test;
void *ehPrimo(void *numero){
    int teste = *((int *)numero);
    int i=0;
    for(i=2; i< teste; i++){
        if(teste % i == 0){
           pthread_exit(NULL); 
        }
    }
    pthread_mutex_lock(&mutexPrimos);
    primos++;
    pthread_mutex_unlock(&mutexPrimos);
    pthread_exit(NULL);
}
void *dividir(void *numero){
    int n = *(int *)numero;
    int i;
    int par = 0;
    for(i=1; i<=n; i++){
        if(n % i == 0){
            par++;
        }
    }
    pthread_mutex_unlock(&mutexDivisores);
    if(par > divisores){
        divisores = par;
        nmaior = n;
    }
    pthread_mutex_unlock(&mutexDivisores);
    pthread_exit(NULL);
}
void *loopp(void *estrutura){
    test teste = *((test *)estrutura);
    int i;
    pthread_t verificaprimo;
    for(i = teste.a; i<teste.b; i++){
        if(i == 1){
            continue;
        }
        int rc = pthread_create(&verificaprimo, NULL, ehPrimo, (void *)&i);
        if(rc){
            printf("deu merda");
            pthread_exit(NULL);
        }
        pthread_join(verificaprimo, NULL);
    }
    pthread_exit(NULL);
}
void *loopd(void *estrutura){
    test teste = *((test *)estrutura);
    int i;
    pthread_t verificadivisor;
    for(i = teste.a; i<teste.b; i++){
        if(i == 1){
            continue;
        }
        int rc = pthread_create(&verificadivisor, NULL, dividir, (void *)&i);
        if(rc){
            printf("deu merda");
            pthread_exit(NULL);
        }
        pthread_join(verificadivisor, NULL);
    }
    pthread_exit(NULL);
}
int main(){
    int a, b;
    scanf("%i%i", &a, &b);
    test *vaifi;
    vaifi = (test*)malloc(sizeof(test));
    vaifi->a = a;
    vaifi->b = b;
    pthread_t threadloopd;
    pthread_t threadloopp;
    int ret = pthread_create(&threadloopd, NULL, loopd, (void *)vaifi);
    if(ret){
        printf("deu merda\n");
        pthread_exit(NULL);
    }
    ret = pthread_create(&threadloopp, NULL, loopp, (void *)vaifi);
    if(ret){
        printf("deu merda\n");
        pthread_exit(NULL);
    }
    pthread_join(threadloopp, NULL);
    pthread_join(threadloopd, NULL);
    printf("%i ", primos);
    printf("%i ", divisores);
    printf("%i\n", nmaior);
    return 0;
}