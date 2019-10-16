#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* itoa(int val, int base){//Função para passar de inteiro para char
	
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
pthread_mutex_t somadosArquivos;

int lidos[100];
double soma = 0;
int con = 0;

void *thread(){
    double meuArquivoSoma = 0; //Cada arquivo enquanto vai lendo, vai somando nessa variavel local
    while(1){
        int i;
        int arquivoaserlido = -1;
        for(i=0; i<A; i++){//Loop para encontrarmos o arquivo a ser lido
            pthread_mutex_lock(&verarquivos);
            if(lidos[i] == 0){
                lidos[i] = 1;
                arquivoaserlido = i;
                pthread_mutex_unlock(&verarquivos);
                break;
            }
            pthread_mutex_unlock(&verarquivos);
        }
        if(arquivoaserlido == -1){//Se não tem nenhum arquivo para ser lido, terminamos nossa execução
            pthread_mutex_lock(&somadosArquivos);//Travamos o mutex para que possamos somar na variavel de soma global
            soma += meuArquivoSoma;
            pthread_mutex_unlock(&somadosArquivos);
            pthread_exit(NULL);//Terminamos a exec
        }
        arquivoaserlido++;
        FILE *arq, *arq2;
        //Trecho de conversão para ler o arquivo certo
        char nomearq[100] = "banco";
        char aux[100];
        char *inteiro;
        inteiro = itoa(arquivoaserlido, 10);
        strcat(nomearq, inteiro);
        strcpy(aux, nomearq);
        strcat(nomearq, ".txt");

        arq = fopen(nomearq, "r+");//Abrimos o arquivo de leitura
        if(arq == NULL){
            printf("Não foi possível terminar a execução\n");
            pthread_exit(NULL);
        }
        strcat(aux, "bcp.txt");
        arq2 = fopen(aux, "w");//Abrimos o arquivo de backup
        if(arq2 == NULL){
            printf("Não foi possível terminar a execução\n");
            pthread_exit(NULL);
        }
        usuario lido; // Variavel que guarda usuário lido
        int c;
        while(fscanf(arq, " %s %s %i %lf", lido.nome, lido.id, &lido.lastacess, &lido.pont) != EOF){//Enquanto estiver algo para ler, lemos
            if(con == 0){//Se o seletor for 0, iremos apenas ler os valores e somar à variável local
                meuArquivoSoma += lido.lastacess/(lido.pont * lido.pont);
            }
            else{//Se for 1, verificamos esse usuário
                if((double)lido.lastacess/(lido.pont * lido.pont) > 2*soma){// Se a conta dele for maior, printamos e não colocamos no arquivo
                    printf("%s\n", lido.nome);
                }
                else{//Escrevemos o usuário no arquivo de backup
                    fprintf(arq2,"%s %s %i %.2lf\n", lido.nome, lido.id, lido.lastacess, lido.pont);
                }
            }
        }
        fclose(arq);//Fechamos o arquivo
        fclose(arq2);
    }
}

int main(int argc, char *argv[]){
    scanf("%i %i %i", &N, &A, &T);
    int i;
    pthread_t threads[T];
    
    pthread_attr_t atributes; //Atributos de cada thread para que elas sejam "joinables"
    pthread_attr_init(&atributes);//
    pthread_attr_setdetachstate(&atributes, PTHREAD_CREATE_JOINABLE);//

    for(i=0; i<T; i++){//Criamos as T threads
        int rc = pthread_create(&threads[i], &atributes, thread, NULL);
        if(rc){
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<T; i++){//Esperamos as threads acabarem a execução
        pthread_join(threads[i], NULL);
    }
    for(i=0; i<A; i++){//Resetamos o vetor que determina se um arquivo já foi lido ou não
        lidos[i] = 0;
    }
    con = 1;//Mudamos o "seletor" das threads
    soma /= N;//Terminamos o cálculo da média
    for(i=0; i<T; i++){//Recriamos as threads mas agora com a função de ler e tirar as pessoas dos arquivos
        int rc = pthread_create(&threads[i], &atributes, thread, NULL);
        if(rc){
            printf("Não foi possível criar a thread\n");
            pthread_exit(NULL);
        }
    }
    for(i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }
    for(i=1; i<=A; i++){//Loop para que possamos mudar o nome dos arquivos
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