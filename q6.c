#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define TAM_buffer 100
#define nThreads 5

typedef struct parametro{
    int valor;
    int id;
    void*(*func_ptr)(void *);
}parametro;

pthread_mutex_t mutexes[TAM_buffer];
pthread_mutex_t mutexFila;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;

int bufferRes[TAM_buffer];
parametro bufferFila[TAM_buffer];
int contadorBuffer=0;
int amount = 0;

void *funexec(void * a){
    parametro b = *(parametro *)a;
    pthread_mutex_lock(&mutexes[b.id]);
    printf("ca %i %i\n", b.id, b.valor);
    bufferRes[b.id] = b.valor + 10;
    pthread_mutex_unlock(&mutexes[b.id]);
    pthread_exit(NULL);
}

int pegarResultadoExecucao(int id){
    int ret;
    pthread_mutex_lock(&mutexes[id]);

    printf("valor %i\n", bufferRes[id]);
    ret = bufferRes[id];

    pthread_mutex_unlock(&mutexes[id]);
    return ret;
}
int agendarExecucao(void*para){
    parametro prm = *((parametro*)para);

    pthread_mutex_lock(&mutexFila);
    
    prm.id = contadorBuffer;
    bufferFila[contadorBuffer] = prm;
    printf("%i\n", prm.id);
    contadorBuffer++;
    amount++;
    pthread_cond_signal(&cheio);
    pthread_mutex_unlock(&mutexFila);
    return prm.id;
}
void *despacha(){
    pthread_t threads[nThreads];
    int contador = 0;
    int atual = 0;
    while(1){
        pthread_mutex_lock(&mutexFila);
        if(atual >= amount){
            printf("testa\n");
            pthread_cond_wait(&cheio, &mutexFila);
        }
        pthread_create(&threads[contador], NULL, bufferFila[atual].func_ptr, (void*)&bufferFila[atual]);
        contador++;
        printf("nem chega %i\n", atual);
        atual++;
        pthread_mutex_unlock(&mutexFila);
    }
}
int main(){
    pthread_t despachante;
    parametro teste;
    
    int i;
    pthread_create(&despachante, NULL, despacha, NULL);
    for(i = 0; i<4; i++){
        teste.func_ptr = funexec;
        teste.valor = i;
        teste.id = agendarExecucao(&teste);
        pthread_cond_signal(&cheio);
        printf("tinha isso la %i\n", pegarResultadoExecucao(teste.id));
    }
    pthread_join(despachante, NULL);
    pthread_exit(NULL);
}