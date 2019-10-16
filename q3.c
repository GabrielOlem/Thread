#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_VARIAVEIS 2
#define P 50

double x[NUM_VARIAVEIS][P];
double b[NUM_VARIAVEIS] = {11, 13};
double a[NUM_VARIAVEIS][NUM_VARIAVEIS] = {2, 1, 5, 7};
int **respo;
int NUM_THREADS;
pthread_barrier_t barrier;

void *conta(void *threadid){
    int k = 0;
    int id = *(int*)threadid;
    while(k < P){//Número de iterações que faremos
        int contador = 0;
        int novoId = respo[id][contador];//pegamos a primeira variável que precisamos calcular
        while(1){
            double somatorio = 0;
            int j;
            for(j=0; j<NUM_VARIAVEIS; j++){//Calculamos o somatório
                if(j != novoId){
                    somatorio += a[novoId][j]*x[j][k];
                }
            }
            x[novoId][k+1] = (1/a[novoId][novoId])*(b[novoId] - somatorio);//Atualizamos o valor da variável 
            contador++;//Aumentamos para pegar a próxima variável
            if(contador > NUM_VARIAVEIS){//Se já passamos do número de variáveis passamos para a próximo iteração
                break;
            }
            novoId = respo[id][contador];//Pegamos a nova variável
            if(novoId == -1){//Se ela for -1 é pq já terminamos as variáveis que temos que calcular então paramos
                break;
            }
        }
        k++;
        pthread_barrier_wait(&barrier);//Esperamos na barreira pelas outras threads
    }
}

int main(int argc, char *argv[]){
    scanf("%i", &NUM_THREADS);
    if(NUM_THREADS > NUM_VARIAVEIS){//Se tivermos mais threads do que variáveis, limitamos os número de threads
        NUM_THREADS = NUM_VARIAVEIS;
    }
    respo = (int **) malloc(sizeof(int*)*NUM_THREADS); //Vetor que armazena as variáveis que cada thread é responsável
    int c;
    for(c=0; c<NUM_THREADS; c++){//Loop para alocar e resetar o vetor
        respo[c] = (int*)malloc(sizeof(int)*NUM_VARIAVEIS);
        int d = 0;
        for(d=0; d<NUM_VARIAVEIS; d++){
            respo[c][d] = -1;
        }
    }
    int a;
    int contad = 0;
    int filtro = 0;
    for(a=0; a<NUM_VARIAVEIS; a++){//Designamos cada variável para uma thread de forma mais justa possível
        contad = a % NUM_THREADS;
        filtro = floor(a / NUM_THREADS);
        respo[contad][filtro] = a;
    }
    pthread_t thread[NUM_THREADS];
    int *ids[NUM_THREADS];
    int i;
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);//Iniciamos a barreira
    for(i=0; i<NUM_THREADS; i++){//Criamos cada uma das threads
        ids[i] = (int*) malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&thread[i], NULL, conta, (void*)ids[i]);
        
    }
    int numerito = NUM_THREADS;
    for(i = 0; i < numerito; i++) {//Loop para esperarmos o fim das threads
        printf("%i\n", numerito);
        pthread_join(thread[i],NULL);
    }
    for(i = 0; i < NUM_VARIAVEIS; i++) {//Printamos o resultado final
        printf("%lf ", x[i][P-1]);
    }
    printf("\n");
    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
}