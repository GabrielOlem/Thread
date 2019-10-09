#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define TAM_buffer 100
#define nThreads 5

typedef struct parametro{
    int valor;
    void*(*func_ptr)(void*);
}parametro;
pthread_mutex_t mutexes[TAM_buffer];

int bufferRes[TAM_buffer];
int bufferFila[TAM_buffer];

int contador=0;
void *funexec(void* para){
    parametro prm = *((parametro*)para);
    printf("a %i\n", prm.valor);
    pthread_exit(NULL);
}

void *pegarResultadoExecucao(void *id){
    long prm = *((long*)id);
    pthread_mutex_lock(&mutexes[prm]);
    printf("valor %i\n", bufferRes[prm]);
    pthread_mutex_unlock(&mutexes[prm]);
    pthread_exit(NULL);
}
void *funcao(void*para){
    parametro prm = *((parametro*)para);
    pthread_t a;
    printf("a\n");
    pthread_create(&a, NULL, prm.func_ptr(para), NULL);
    pthread_join(a, NULL);
    pthread_exit(NULL);
}
int main(){

    pthread_t teste;
    parametro *a;
    a->valor = 14;
    a->func_ptr = funexec;
    pthread_create(&teste, NULL, funcao, (void*)a);
    pthread_join(teste, NULL);
    pthread_exit(NULL);
}