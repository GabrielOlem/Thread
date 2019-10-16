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
//As matrizes esparsas foram implementadas de forma que ao fim de cada linha é necessário colocar um ponto com indíce -1
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
int resultadoop2[linhas][colunas2] = {0};
int resultadoop3[linhas][colunas2] = {0};

void *linhacoluna(void *numero){//Função para calcular a matriz esparsa pelo vetor comum
    long linha = *((long *)numero);
    int k = 0;
    int soma = 0;
    while(1){//Calculamos a linha passado pelo vetor comum
        if(matrizesparsa[linha][k].index == -1){//Se não tiver mais pontos na linha terminamos a execução
            break;
        }
        soma += matrizesparsa[linha][k].numero*vetorcomum[matrizesparsa[linha][k].index];//Somamos o valor do ponto da matriz esparsa pelo equivalente no vetor comum
        k++;
    }
    resultadoop1[linha] = soma; //Armazenamos o valor na matriz resposta
    pthread_exit(NULL);
}
void *oper1(){
    pthread_t tlinhas[linhas];//Criamos uma thread para cada calcular cada linha
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna, (void *)id);//Criamos as threads que executa cada linha
        if(rc){
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<linhas; i++){//Esperamos as threads acabarem
        pthread_join(tlinhas[i], NULL);
    }
    pthread_exit(NULL);
}
void *linhacoluna2(void *numero){
    long linha = *((long *)numero);
    int j = 0;
    for(j=0; j<colunas2; j++){//Calculamos a linha passada por cada coluna da matriz comum
        int k = 0;
        while(1){//Calculo da multiplicação da linha pela coluna j;
            if(matrizesparsa[linha][k].index == -1){//Se não tiver mais pontos na linha terminamos
                break;
            }
            resultadoop2[linha][j] += matrizesparsa[linha][k].numero*matrizcomum[matrizesparsa[linha][k].index][j];
            k++;
        }
    }
    pthread_exit(NULL);
}
void *oper2(){
    pthread_t tlinhas[linhas];//Criamos uma thread para cada calcular cada linha
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna2, (void *)id);//Criamos as threads que executa cada linha
        if(rc){
            printf("Não foi possível criar a thread\n");
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
    for(j=0; j<colunas2; j++){//Loop para multiplicar a linha passada por cada coluna da matriz esparsa
        int k = 0;
        while(1){
            if(matrizesparsa[linha][k].index == -1){//Se não tiver mais elementos na linha passada, passamos para o cálculo da próxima coluna
                break;
            }
            int coluna = -1;
            int l = 0;
            int valor = 0;
            while(1){//Loop para encontrarmos o valor equivalente na matriz esparsa2 
                if(matrizesparsa2[matrizesparsa[linha][k].index][l].index == -1){//Caso não encontremos a variável na outra matriz
                    break;
                }
                if(matrizesparsa2[matrizesparsa[linha][k].index][l].index == j){//Se encontramos o correspondente na outra matriz
                    coluna = 0;
                    valor = matrizesparsa2[matrizesparsa[linha][k].index][l].numero; //Salvamos o valor 
                    break;
                }
                l++;
            }
            if(coluna != -1){//Se tivermos encontrado o corresponte, calculamos, senão só passamos direto, pois é como se estivéssemos fazendo algum valor vezes 0
                resultadoop3[linha][j] += matrizesparsa[linha][k].numero*valor;
            }
            k++;
        }
    }
    pthread_exit(NULL);
}
void *oper3(){
    pthread_t tlinhas[linhas];//Criamos uma thread para cada calcular cada linha
    long *id;
    int i;
    for(i=0; i<linhas; i++){
        id = (long*)malloc(sizeof(long));
        *id = i;
        int rc = pthread_create(&tlinhas[i], NULL, linhacoluna3, (void *)id);
        if(rc){
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<linhas; i++){ 
        pthread_join(tlinhas[i], NULL);
    }
    pthread_exit(NULL);
}
int main(){
    pthread_t op1, op2, op3;//Cada thread é responsável por uma operação
    pthread_create(&op1, NULL, oper1, NULL);
    pthread_join(op1, NULL);
    pthread_create(&op2, NULL, oper2, NULL);
    pthread_join(op2, NULL);
    pthread_create(&op3, NULL, oper3, NULL);
    pthread_join(op3, NULL);
    int i, j;
    printf("Resultado 1\n");
    for(i=0; i<linhas; i++){
        printf("%i\n", resultadoop1[i]);
    }
    printf("Resultado 2\n");
    for(i=0; i<linhas; i++){
        for(j=0; j<colunas2; j++){
            printf("%i ", resultadoop2[i][j]);
        }
        printf("\n");
    }
    printf("Resultado 3\n");
    for(i=0; i<linhas; i++){
        for(j=0; j<colunas2; j++){
            printf("%i ", resultadoop3[i][j]);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}