#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define linhas 3
#define colunas 4
#define linhas2 4
#define colunas2 2

typedef struct ponto{
    int index;
    int numero;
}ponto;

ponto matrizesparsa[linhas][colunas] = {{{0,2}, {1,1}, {-1,-1}}, 
                                        {{0,-1}, {1,2}, {2,-1}, {-1,-1}},
                                        {{1,-1}, {2,2}, {3,-1}, {-1,-1}}};

ponto matrizesparsa2[linhas2][colunas2] = {{{0,1},{-1,1}}, 
                                        {{-1,-1}},
                                        {{-1,-1}}, 
                                        {{1,2},{-1,-1}}};
int vetorcomum[linhas] = {2, 1, 1};
int matrizcomum[linhas][colunas] = {{1, 1, 1, 1}, {1, 2, 1, 1}, {3, 4, 2, 1}};
int resultadoop1[linhas];
int resultadoop2[linhas][colunas] = {0};
int resultadoop3[linhas][colunas] = {0};

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
    for(j=0; j<colunas2; j++){
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
void *linhacoluna3(void *numero){
    long linha = *((long *)numero);
    int j = 0;
    for(j=0; j<colunas2; j++){
        int k = 0;
        while(1){
            if(matrizesparsa[linha][k].index == -1){
                break;
            }
            int coluna = -1;
            int l = 0;
            int valor = 0;
            while(1){
                if(matrizesparsa2[matrizesparsa[linha][k].index][l].index == -1){
                    break;
                }
                if(matrizesparsa2[matrizesparsa[linha][k].index][l].index == j){
                    coluna = 0;
                    valor = matrizesparsa2[matrizesparsa[linha][k].index][l].numero;
                    printf("%i %i\n", matrizesparsa[linha][k].index, l);
                    break;
                }
                l++;
            }
            if(coluna != -1){
                printf("valor %i %i %li\n", valor, matrizesparsa[linha][k].numero, linha);
                resultadoop3[linha][j] += matrizesparsa[linha][k].numero*valor;
            }
            k++;
        }
    }
    pthread_exit(NULL);
}
void *oper3(){
    pthread_t tlinhas[linhas];
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna3, (void *)id);
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
    pthread_create(&op3, NULL, oper3, NULL);
    pthread_join(op3, NULL);
    int i, j;
    for(i=0; i<linhas; i++){
        for(j=0; j<colunas2; j++){
            printf("%i ", resultadoop3[i][j]);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}