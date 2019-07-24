#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//semaforo binario de exclusión mutua que controla el acceso a la variable 
//compartida readersy sirve de barrera para los escritores
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

//semaforo binario de exclusión mutua para los escritores, tambien lo utiliza
// el primer y ultimo lector para entrar y salir de la seccion critica
// pero no será utilizada mientra haya otros lectores o escritores en la sc
pthread_mutex_t writer = PTHREAD_MUTEX_INITIALIZER;

//número de lectores en la sección crítica
int readers;
//representa el libro sobre el que el escritor escribe su numero
int book = -1;


void *read(void *id){
    //cada lector lee 10 veces
    for (unsigned int j = 0; j < 10; j ++){

        // Lector lee, se bloquea el acceso a la variable readers para el resto, sc
        pthread_mutex_lock(&mx);
        // Se incrementa readers
        readers ++;
        if(readers == 1){
            //Si somos el primer lector bloqueamos el aceso a los escritores
            pthread_mutex_lock(&writer);
        }
        // se desbloquea el acceso a la variable readers, sc
        pthread_mutex_unlock(&mx);
        
        printf("El lector %d ha leido un valor de %d\n", *(int*)id, book);

        //El lector ha terminado de leer, bloqueamos el acceso a la sc, la variable readers
        pthread_mutex_lock(&mx);
        //decrementamos readers, en estos momentos hay un lector menos
        readers --;
        if(readers == 0){
            //Si no hay ningun lector, desbloqueamos el aceso a los escritores
            pthread_mutex_unlock(&writer);
        }
        //terminamos con la sc, devolvemos el acceso a la sc a quien la quiera usar
        pthread_mutex_unlock(&mx);

    }

    pthread_exit(id);
}

void *write(void *id){
    //cada escritor escribe 5 veces
    for (unsigned int j = 0; j < 5; j ++){

        //exclusion mutua escritores
        pthread_mutex_lock(&writer);

        //actualizamos el valor del libro
        book = *(int*)id;
        printf("El escritor %d ha escrito en el libro\n", book);

        //abrimos el acceso a la variable book, exclusion mutua
        pthread_mutex_unlock(&writer);

    }
    pthread_exit(id);
}

//función principal
void main() {

    int *salida;
    int error;

    unsigned int numreaders = 10;
    unsigned int numwriters = 5;

    //declaramos los hilos
    pthread_t tidreader[numreaders], tidwriter[numwriters];

    unsigned int i;

    for ( i = 0; i < numwriters; i ++){
        error = pthread_create(&tidwriter[i], NULL, write, &i);
        if (error){
            fprintf(stderr, "Error: %d: %s\n", error, strerror (error));
            exit(-1);
        }
    }

    for ( i = 0; i <numreaders; i ++){
        error = pthread_create(&tidreader[i], NULL, read, &i);
        if (error){
            fprintf(stderr, "Error: %d: %s\n", error, strerror (error));
            exit(-1);
        }
    }
    


    //join the threads
    for ( i = 0; i < numreaders; i++){
        error = pthread_join(tidreader[i], (void**)&salida);
        if (error)
            fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
    }
    for ( i = 0; i < numwriters; i++){
        error = pthread_join(tidwriter[i], (void**)&salida);
        if (error)
            fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
    }

}