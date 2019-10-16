#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int primos = 0;
pthread_mutex_t mutexPrimos;//Mutex para acessar a variável primos
pthread_mutex_t mutexDivisores;//Mutex para acesssar a variável de divisores
int divisores = 0;
int nmaior = 0;

typedef struct test{
    int a, b;
}test;
void *ehPrimo(void *numero){
    int teste = *((int *)numero);
    int i=0;
    for(i=2; i< teste; i++){
        if(teste % i == 0){//Se o número não for primo, saímos
           pthread_exit(NULL); 
        }
    }
    pthread_mutex_lock(&mutexPrimos);//Se for primo, aumentamos a quantidade na variável de primos
    primos++;
    pthread_mutex_unlock(&mutexPrimos);
    pthread_exit(NULL);
}
void *dividir(void *numero){
    int n = *(int *)numero;
    int i;
    int par = 0;//Variável que guarda o número de divisores
    for(i=1; i<=n; i++){
        if(n % i == 0){
            par++;
        }
    }
    pthread_mutex_unlock(&mutexDivisores);
    if(par > divisores){//Se o valor desse número for maior que o maior atual, atualizamos
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
    for(i = teste.a; i<teste.b; i++){//Loop que rodamos o intervalo e verificando se aquele número é primo
        if(i == 1){//Se for 1 não fazemos nada
            continue;
        }
        int rc = pthread_create(&verificaprimo, NULL, ehPrimo, (void *)&i);//Criamos a thread que verifica se é primo
        if(rc){
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
        pthread_join(verificaprimo, NULL);//Esperamos a thread terminar
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
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
        pthread_join(verificadivisor, NULL);
    }
    pthread_exit(NULL);
}
int main(){
    int a, b;
    scanf("%i%i", &a, &b);//lemos o intervalo
    test *vaifi; // Estrutura que armazena o intervalo para que possamos passar para as threads
    vaifi = (test*)malloc(sizeof(test));
    vaifi->a = a;
    vaifi->b = b;
    pthread_t threadloopd;
    pthread_t threadloopp;
    int ret = pthread_create(&threadloopd, NULL, loopd, (void *)vaifi); //Criamos a thread que faz o loop de divisor
    if(ret){
        printf("Não foi possível criar a thread\n");
        pthread_exit(NULL);
    }
    ret = pthread_create(&threadloopp, NULL, loopp, (void *)vaifi); //Criamos a thread que faz o loop de primo
    if(ret){
        printf("Não foi possível criar a thread\n");
        pthread_exit(NULL);
    }
    pthread_join(threadloopp, NULL);//Esperamos as threads acabarem
    pthread_join(threadloopd, NULL);
    printf("%i ", primos); //Printamos os resultados
    printf("%i ", divisores);
    printf("%i\n", nmaior);
    return 0;
}