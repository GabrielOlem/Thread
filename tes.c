#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
void *thread(){
    printf("teste\n");
    pthread_exit(NULL);
}
int main(){
    int T;
    scanf("%i", &T);
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
    pthread_exit(NULL);
}