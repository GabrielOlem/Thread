#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int contador = 0;
int acabou1 = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; //Mutex que "fiscaliza" a variável contador

void *conta(void *threadid){
    long tid = *((long *)threadid);
    while(1){
        pthread_mutex_lock(&mymutex); //Travamos o mutex pois vamos verificar o contador
        if(contador == 1000000){ //Se o contador já estiver acabado
            if(acabou1 == 1){ //Se alguma outra thread acabou
                pthread_mutex_unlock(&mymutex); //só destravamos
                pthread_exit(NULL); // e saímos
            }
            acabou1 = 1;//Avisamos as outras threads que já acabamos
            printf("Fim %i Thread %li\n", contador, tid);
            pthread_mutex_unlock(&mymutex);//Destravamos o mutex
            pthread_exit(NULL);// e saímos
        }
        contador++;//Caso não tenha acabado, só somamos o contador
        pthread_mutex_unlock(&mymutex);//Destravamos o mutex
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
        int rc = pthread_create(&thread[i], NULL, conta, (void *)taskids[i]); //Criamos N threads que vão fazer a contagem
        if(rc){
            printf("Não foi possível criar a Thread\n");
            return -1;
        }
    }
    for(i=0; i<n; i++){ //Loop para aguardar o fim da execução de cada uma das threads
        pthread_join(thread[i], NULL);
    }
    pthread_exit(NULL);
}