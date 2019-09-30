#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define linhas 4
#define colunas 4
typedef struct ponto{
    int index;
    int numero;
}ponto;

ponto matrizesparsa[linhas][colunas] = {{{0,2}, {1,1}, {-1,-1}}, 
                                        {{0,-1}, {1,2}, {2,-1}, {-1,-1}},
                                        {{1,-1}, {2,2}, {3,-1}, {-1,-1}}, 
                                        {{2,-1}, {3,2}, {-1,-1}}};
int vetorcomum[linhas] = {2, 1, 1, 1};
int matrizcomum[linhas][colunas] = {{1, 1, 1, 1}, {1, 2, 1, 1}, {3, 1, 2, 1}, {1, 1, 1, 1}};
int resultadoop1[linhas];
int resultadoop2[linhas][colunas] = {0};

void *linhacoluna(void *numero){
    long linha = *((long *)numero);
    int k = 0;
    int soma = 0;
    while(1){
        if(matrizesparsa[linha][k].index == -1){
            break;
        }
        soma += matrizesparsa[linha][k].numero*vetorcomum[matrizesparsa[linha][k].index];
        k++;
    }
    resultadoop1[linha] = soma;
    pthread_exit(NULL);
}
void *oper1(){
    pthread_t tlinhas[linhas];
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna, (void *)id);
        if(rc){
            printf("PARAPARAPARA\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<linhas; i++){ 
        pthread_join(tlinhas[i], NULL);
    }
    pthread_exit(NULL);
}
void *linhacoluna2(void *numero){
    long linha = *((long *)numero);
    int j = 0;
    for(j=0; j<colunas; j++){
        int k = 0;
        while(1){
            if(matrizesparsa[linha][k].index == -1){
                break;
            }
            resultadoop2[linha][j] += matrizesparsa[linha][k].numero*matrizcomum[matrizesparsa[linha][k].index][j];
            k++;
        }
    }
    pthread_exit(NULL);
}
void *oper2(){
    pthread_t tlinhas[linhas];
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna2, (void *)id);
        if(rc){
            printf("PARAPARAPARA\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<linhas; i++){ 
        pthread_join(tlinhas[i], NULL);
    }
    pthread_exit(NULL);
}
int main(){
    pthread_t op1, op2, op3;
    pthread_create(&op1, NULL, oper1, NULL);
    pthread_join(op1, NULL);
    pthread_create(&op2, NULL, oper2, NULL);
    pthread_join(op2, NULL);
    int i, j;
    for(i=0; i<linhas; i++){
        for(j=0; j<colunas; j++){
            printf("%i ", resultadoop2[i][j]);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}