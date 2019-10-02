#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}
typedef struct usuario{
    char nome[100];
    char id[100];
    int lastacess;
    double pont;
}usuario;

int N;
int A;
int T;
usuario *vetor;
pthread_mutex_t verarquivos;
int lidos[100];

void *thread(){
    int con = 0;
    while(1){
        int i;
        int arquivoaserlido = -1;
        for(i=0; i<A; i++){
            pthread_mutex_lock(&verarquivos);
            if(lidos[i] == 0){
                lidos[i] = 1;
                arquivoaserlido = i;
                pthread_mutex_unlock(&verarquivos);
                break;
            }
            pthread_mutex_unlock(&verarquivos);
        }
        if(arquivoaserlido == -1){
            pthread_exit(NULL);
        }
        arquivoaserlido++;
        FILE *arq;
        char nomearq[100] = "banco";
        char *inteiro;
        inteiro = itoa(arquivoaserlido, 10);
        strcat(nomearq, inteiro);
        strcat(nomearq, ".txt");
        arq = fopen(nomearq, "r");
        if(arq == NULL){
            printf("NAO VAI DAR NAO BAMBAM\n");
            pthread_exit(NULL);
        }
        usuario lido;
        int c;
        char linha[200];
        while(fscanf(arq, " %s %s %i %lf", lido.nome, lido.id, &lido.lastacess, &lido.pont) != EOF){
            int a = atoi(lido.id);
            vetor[a] = lido;
        }
        fclose(arq);
    }
}

int main(int argc, char *argv[]){
    scanf("%i %i %i", &N, &A, &T);
    vetor = (usuario*)malloc(N*sizeof(usuario));
    pthread_t threads[T];
    int i;
    for(i=0; i<T; i++){
        int rc = pthread_create(&threads[i], NULL, thread, NULL);
        if(rc){
            printf("TEM CARBURADOR?\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }
    double soma = 0;
    for(int i=1; i<=N; i++){
        soma += vetor[i].lastacess/(vetor[i].pont*vetor[i].pont);
    }
    soma /= N;
    printf("a %.2lf\n", soma);
    for(int i=1; i<=N; i++){
        if(vetor[i].lastacess/(vetor[i].pont*vetor[i].pont) > 2*soma){
            printf("%s\n", vetor[i].nome);
        }
    }
    pthread_exit(NULL);
}