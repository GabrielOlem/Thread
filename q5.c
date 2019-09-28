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
int vetoresparso[linhas] = {2, 1, 1, 1};
int resultadoop1[linhas];
void *linhacoluna(void *numero){
    long linha = *((long *)numero);
    int k = 0;
    int soma = 0;
    while(1){
        if(matrizesparsa[linha][k].index == -1){
            break;
        }
        soma += matrizesparsa[linha][k].numero*vetoresparso[matrizesparsa[linha][k].index];
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
int main(){
    pthread_t op1, op2, op3;
    pthread_create(&op1, NULL, oper1, NULL);
    pthread_join(op1, NULL);
    int i;
    for(i=0; i<linhas; i++){
        printf("%i\n", resultadoop1[i]);
    }
    pthread_exit(NULL);
}