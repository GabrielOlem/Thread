#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define TAM_buffer 100
#define nThreads 5

typedef struct parametro{
    int valor;
    int id;
    int(*func_ptr)(int);
    int aExec;
}parametro;

pthread_mutex_t mutexes[TAM_buffer];
pthread_mutex_t mutexFila;
pthread_mutex_t mutexVerifica;

pthread_cond_t cheio;
pthread_cond_t result[TAM_buffer];

int checkThreads[nThreads];
int bufferRes[TAM_buffer];
parametro bufferFila[TAM_buffer];
int contadorBuffer=0;
int amount = 0;
void *thread(void * para){
    parametro b = *(parametro *)para;

    pthread_mutex_lock(&mutexes[b.id]);

    bufferRes[b.id] = b.func_ptr(b.valor);

    pthread_cond_signal(&result[b.id]);
    pthread_mutex_unlock(&mutexes[b.id]);

    pthread_mutex_lock(&mutexVerifica);
    checkThreads[b.aExec] = 0;
    printf("Sou a thread %i\n", b.aExec);
    pthread_mutex_unlock(&mutexVerifica);
    
    pthread_exit(NULL);
}
int funexec(int a){
    int i;
    int c = 1;
    int d = 2;
    int j;
    for(i=0; i<1e7; i++){
        for(j=0; j<1e7; j++);
    }
    return a + 10;
}
int pegarResultadoExecucao(int id){
    int ret;
    pthread_mutex_lock(&mutexes[id]);

    while(bufferRes[id] == -1){
        pthread_cond_wait(&result[id], &mutexes[id]);
    }
    ret = bufferRes[id];

    pthread_mutex_unlock(&mutexes[id]);
    return ret;
}
int agendarExecucao(void*para){
    parametro prm = *((parametro*)para);

    pthread_mutex_lock(&mutexFila);
    
    prm.id = contadorBuffer;
    bufferFila[contadorBuffer] = prm;
    contadorBuffer++;
    
    pthread_cond_signal(&cheio);

    pthread_mutex_unlock(&mutexFila);
    return prm.id;
}
void *despacha(){
    pthread_t threads[nThreads];
    int atual = 0;
    while(1){
        int i;
        int aExec = -1;
        while(1){
            for(i=0; i<nThreads; i++){
                pthread_mutex_lock(&mutexVerifica);
                printf("quero ver isso aqui %i\n", i);
                if(checkThreads[i] == 0){
                    printf("\n");
                    checkThreads[i] = 1;
                    aExec = i;
                    pthread_mutex_unlock(&mutexVerifica);
                    break;
                }
                pthread_mutex_unlock(&mutexVerifica);
            }
            if(aExec != -1){
                break;
            }
        }
        pthread_mutex_lock(&mutexFila);
        if(atual >= contadorBuffer){
            pthread_cond_wait(&cheio, &mutexFila);
        }
        bufferFila[atual].aExec = aExec;
        pthread_create(&threads[aExec], NULL, thread, (void*)&bufferFila[atual]);
        atual++;

        pthread_mutex_unlock(&mutexFila);
    }
}
int main(){
    pthread_t despachante;
    parametro teste;
    memset(bufferRes, -1, 100*sizeof(int));
    int i;
    pthread_create(&despachante, NULL, despacha, NULL);
    for(i = 0; i<100; i++){
        teste.func_ptr = funexec;
        teste.valor = i;
        teste.id = agendarExecucao(&teste);
        printf("result %i\n", pegarResultadoExecucao(teste.id));
    }
    pthread_join(despachante, NULL);
    pthread_exit(NULL);
}