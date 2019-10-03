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
pthread_mutex_t verarquivos;
pthread_mutex_t a;

int lidos[100];
double soma = 0;
int con = 0;

void *thread(){
    double meuArquivoSoma = 0;
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
            pthread_mutex_lock(&a);
            soma += meuArquivoSoma;
            pthread_mutex_unlock(&a);
            pthread_exit(NULL);
        }
        arquivoaserlido++;
        FILE *arq, *arq2;
        char nomearq[100] = "banco";
        char aux[100];
        char *inteiro;
        inteiro = itoa(arquivoaserlido, 10);
        strcat(nomearq, inteiro);
        strcpy(aux, nomearq);
        strcat(nomearq, ".txt");
        arq = fopen(nomearq, "r+");
        if(arq == NULL){
            printf("NAO VAI DAR NAO BAMBAM\n");
            pthread_exit(NULL);
        }
        strcat(aux, "bcp.txt");
        arq2 = fopen(aux, "w");
        if(arq2 == NULL){
            printf("NAO VAI DAR NAO BAMBAM\n");
            pthread_exit(NULL);
        }
        usuario lido;
        int c;
        char linha[200];
        while(fscanf(arq, " %s %s %i %lf", lido.nome, lido.id, &lido.lastacess, &lido.pont) != EOF){
            if(con == 0){
                meuArquivoSoma += lido.lastacess/(lido.pont * lido.pont);
            }
            else{
                if((double)lido.lastacess/(lido.pont * lido.pont) > 2*soma){
                    printf("%s\n", lido.nome);
                }
                else{
                    fprintf(arq2,"%s %s %i %.2lf\n", lido.nome, lido.id, lido.lastacess, lido.pont);
                }
            }
        }
        fclose(arq);
        fclose(arq2);
    }
}
void *teste(){
    printf("a");
    pthread_exit(NULL);
}
int main(int argc, char *argv[]){
    scanf("%i %i %i", &N, &A, &T);
    int i;
    pthread_t threads[T];
    
    pthread_attr_t atributes;
    pthread_attr_init(&atributes);
    pthread_attr_setdetachstate(&atributes, PTHREAD_CREATE_JOINABLE);

    for(i=0; i<T; i++){
        int rc = pthread_create(&threads[i], &atributes, thread, NULL);
        if(rc){
            printf("TEM CARBURADOR?\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }
    for(i=0; i<A; i++){
        lidos[i] = 0;
    }
    con = 1;
    soma /= N;
    for(i=0; i<T; i++){
        int rc = pthread_create(&threads[i], &atributes, thread, NULL);
        if(rc){
            printf("TEM CARBURADOR?\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }
    for(i=1; i<=A; i++){
        FILE *arq, *arq2;
        char nomearq[100] = "banco";
        char aux[100];
        char *inteiro;
        inteiro = itoa(i, 10);
        strcat(nomearq, inteiro);
        strcpy(aux, nomearq);
        strcat(nomearq, ".txt");
        strcat(aux, "bcp.txt");
        rename(aux, nomearq);
    }
    printf("%.2lf\n", soma);
    pthread_exit(NULL);
}