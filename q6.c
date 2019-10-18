//A API foi implementada de maneira que, assim que um usuário solicita um agendamento ele precisa pegar logo o resultado, pois como contamos com um buffer limitado, existe o risco do seu dado ser sobrescrito pelo dado de outro usuário
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define TAM_buffer 100 //Buffer com tamanho limitado
#define nThreads 4 //Quantidade de threads

typedef struct parametro{//struct que será passada como parâmetro 
    int valor;
    int id;
    int(*func_ptr)(int);
    int aExec;
}parametro;

pthread_mutex_t mutexes[TAM_buffer]; //Mutex para acessar o vetor BufferRes
pthread_mutex_t mutexFila; //Mutex para modificar a fila
pthread_mutex_t mutexVerifica; //Mutex que verifica threads

pthread_cond_t cheio; //Variável de condição para avisar se a despachante precisa voltar
pthread_cond_t result[TAM_buffer]; //Variável que determina se um resultado já acabou ou não

int checkThreads[nThreads];
int bufferRes[TAM_buffer];
parametro bufferFila[TAM_buffer];
int contadorBuffer=0;
int amount = 0;

void *thread(void * para){
    parametro b = *(parametro *)para;

    pthread_mutex_lock(&mutexes[b.id]);//Travamos o mutex para ver o resultado

    bufferRes[b.id] = b.func_ptr(b.valor);//guardamos o valor da função a ser executada

    pthread_cond_signal(&result[b.id]);//Avisamos a variável de condição que a execução acabou

    pthread_mutex_unlock(&mutexes[b.id]);//Destravamos o mutex

    pthread_mutex_lock(&mutexVerifica);//Avisamos que essa thread já acabou sua execução
    printf("sou thread %i\n", b.aExec);
    checkThreads[b.aExec] = 0;
    pthread_mutex_unlock(&mutexVerifica);
    
    pthread_exit(NULL);
}

int funexec(int a){//Funções que podem ser executadas
    return a + 10;
}
int funexec2(int a){
    return a + 100;
}

int pegarResultadoExecucao(int id){
    int ret;
    pthread_mutex_lock(&mutexes[id]);//Travamos para verificar o resultado

    if(bufferRes[id] == -1){//Se não tiver o resultado ainda, esperamos o resultado
        pthread_cond_wait(&result[id], &mutexes[id]);
    }
    ret = bufferRes[id];//Guardamos o resultado
    bufferRes[id] = -1;//E limpamos aquela posição

    pthread_mutex_unlock(&mutexes[id]);//Destravamos o mutex
    return ret;
}
int agendarExecucao(void*para){
    parametro prm = *((parametro*)para);
    pthread_mutex_lock(&mutexFila);//Travamos o mutex da fila

    prm.id = contadorBuffer;//Guardamos o id daquela execução
    bufferFila[contadorBuffer] = prm; //Guardamos a execução na fila
    contadorBuffer++;
    if(contadorBuffer >= TAM_buffer){//Se o contador chegou ao máximo resetamos a fila e o contador
        int i = 0;
        for(i = 0; i<TAM_buffer; i++){
            bufferRes[i] = -1;
        }
        contadorBuffer = 0;
    }

    pthread_cond_signal(&cheio);//Avisamos a despachante que ela precisa executar

    pthread_mutex_unlock(&mutexFila);//Destravamos o mutex
    return prm.id;//Retornamos o id daquela execução
}
void *despacha(){
    pthread_t threads[nThreads];
    int atual = 0;
    while(1){//Loop infinito
        int i;
        int aExec = -1;//Variavel que determina qual thread vai executar
        while(1){//Loop que procuramos a próxima thread que irá executar
            for(i=0; i<nThreads; i++){
                pthread_mutex_lock(&mutexVerifica);
                if(checkThreads[i] == 0){//Se a thread não estiver sendo executada
                    checkThreads[i] = 1;//Dizemos que ela está sendo executada
                    aExec = i;
                    pthread_mutex_unlock(&mutexVerifica);
                    break;
                }
                pthread_mutex_unlock(&mutexVerifica);
            }
            if(aExec != -1){
                break;
            }
        }
        pthread_mutex_lock(&mutexFila); //Travamos a fila 
        if(atual >= TAM_buffer){ //Se atual já estiver passado do tamanho do buffer, voltamos para o começo
            atual = 0;
        }
        if(atual >= contadorBuffer){ //Se não tiver mais funções a serem executadas, dormimos
            pthread_cond_wait(&cheio, &mutexFila);
        }
        bufferFila[atual].aExec = aExec;
        pthread_create(&threads[aExec], NULL, thread, (void*)&bufferFila[atual]);//Criamos a thread que vai executar aquela função
        atual++;

        pthread_mutex_unlock(&mutexFila);
    }
}
int main(){
    pthread_t despachante;
    parametro teste;
    memset(bufferRes, -1, 100*sizeof(int));
    int i;
    pthread_create(&despachante, NULL, despacha, NULL);//Criamos a despanchante
    for(i = 0; i<1000; i++){
        if(i % 2 == 0){
            teste.func_ptr = funexec;
        }
        else{
            teste.func_ptr = funexec2;
        }
        teste.valor = i;
        teste.id = agendarExecucao(&teste); //Agendamos a execução
        printf("result %i\n", pegarResultadoExecucao(teste.id));//Logo em seguida pegamos o resultado
    }
    pthread_join(despachante, NULL);
    pthread_exit(NULL);
}