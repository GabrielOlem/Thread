#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define TAM_buffer 100
#define nThreads 5

typedef struct parametro{
    int valor;
    int id;
    void*(*func_ptr)(int);
}parametro;

pthread_mutex_t mutexes[TAM_buffer];
pthread_mutex_t mutexFila;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;

int bufferRes[TAM_buffer];
parametro bufferFila[TAM_buffer];
int contadorBuffer=0;
int amount = 0;

void *funexec(int valor){
    printf("ca %i\n", valor);
    pthread_exit(NULL);
}

int pegarResultadoExecucao(void *id){
    long prm = *((long*)id);
    int ret;
    pthread_mutex_lock(&mutexes[prm]);
    printf("valor %i\n", bufferRes[prm]);
    ret = bufferRes[prm];
    pthread_mutex_unlock(&mutexes[prm]);
    return ret;
}
int agendarExecucao(void*para){
    parametro prm = *((parametro*)para);
    pthread_mutex_lock(&mutexFila);
    bufferFila[contadorBuffer] = prm;
    prm.id = contadorBuffer;
    contadorBuffer++;
    amount++;
    printf("%i\n", contadorBuffer);
    pthread_mutex_unlock(&mutexFila);
    return prm.id;
}
void *despacha(){
    pthread_t threads[nThreads];
    int contador = 0;
    while(1){
        pthread_mutex_lock(&mutexFila);
        if(amount == 0){
            pthread_cond_wait(&cheio, &mutexFila);
        }
        pthread_create(&threads[contador], NULL, bufferFila[amount-1].func_ptr(bufferFila[amount-1].valor), NULL);
        pthread_join(threads[contador], NULL);
        amount--;
        contador++;
        printf("teste\n");
        pthread_mutex_unlock(&mutexFila);
    }
}
int main(){
    pthread_t despachante;
    parametro teste;
    teste.func_ptr = funexec;
    teste.valor = 666;
    teste.id = agendarExecucao(&teste);
    teste.func_ptr = funexec;
    teste.valor = 665;
    teste.id = agendarExecucao(&teste);
    int i;
    pthread_create(&despachante, NULL, despacha, NULL);
    pthread_join(despachante, NULL);
    pthread_exit(NULL);
}