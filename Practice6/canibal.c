#include <stdio.h>
#include  <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



//numero de misioneros simultaneos en la olla
#define M   3
// cerrojo para proteger la exclusion mutua sobre la variable olla
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//variable que representa la olla, inicializada a su máxima capacidad
int     olla = M;

//semaforo utilizado para hacer que el salvaje se detenga hasta que el 
//cocinero llene la olla inicializado a 0
sem_t   espera;

//semaforo inicializado a 0 y usado para que el cocinero no haga nada 
//cuando la olla no vacia
sem_t   coci;


void *cocinar(void *id){
    //bucle infinito, el cocinero espera siempre que se le llame a llenar la olla
    while(1){
        //PROTOCOLO DE ENTRADA
        //el cocinero espera a ser llamado, su semaforo esta en wait
        // hasta que se haga un post que indique que ya puede cocinar
        sem_wait(&coci);
        //mutex se encuentra bloqueado por canibal en comer
        // por lo tanto el acceso a la SC es seguro
        //SC
        // Se actualiza la variable olla a su máxima capacidad
        olla = M;

        //PROTOCOLO DE SALIDA

        printf("Cocinero acaba de llenar la olla\n");
        //ya se puede comer, el cocinero permite que 
        // quien hicera un wait, para comer, ya pueda acceder
        sem_post(&espera);
    }
    pthread_exit(NULL);

}


void *comer(void *id){

    //variable que almacena el id del canibal
    int i = *(int*)id;
    //bucle infinito para que el canibal quiera comer siempre
    while(1){
        //pequeño retardo de 1 segundo para ver el resultado por consola bien
            sleep(1);

        //PROTOCOLO DE ENTRADA A LA SC
        pthread_mutex_lock(&mutex);
        //si la olla se encuentra vacia, el canibal no puede comer
        if(olla == 0){
            //se llama al cocinero, haciendose post, el cocinero,
            // que espera con un wait podrá cocinar
            sem_post(&coci);
            //el canibal espera a que el cocinero haga un post del 
            //semaforo de espera, avisandole de que la olla ya esta llena
            sem_wait(&espera);
        }
        
        //decrementamos el contenido de la olla, canibal come
        olla -= 1;
        printf("Canibal %d come de la olla\n", i);
        
        //PROTOCOLO DE SALIDA DE LA SC
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(id);
}

int main(int argc, char *argv[]){

    int *salida;
    int error;
    //número de canibales
    unsigned int numcanibales = 10;
    //creamos canibales y cocinero
    pthread_t tidcocinero, tidcanibal[numcanibales];

    //iNICIALIZACION DE SEMAFOROS, primer valor indica que son hilos de un proceso, 
    //en el segundo se inicializan a 0
    //semaforo usado parada hacer que el salvaje se detenga hasta que el cocinero 
    //llene la olla
    sem_init(&espera, 0, 0);
    //semaforo usado para que el cocinero no haga nada cuando la olla no esta vacia
    sem_init(&coci, 0, 0);

    //CREACION DE LOS HILOS
    //En primer lugar el cocinero que usa la función cocinar
    error = pthread_create(&tidcocinero, NULL, cocinar, NULL);
    if (error){
        fprintf(stderr, "Error: %d: %s\n", error, strerror(error) );
        exit(-1);
    }

    //en segundo lugar los canibales, van a ser 10
    unsigned int i;
    for(i = 0; i < numcanibales; i++){

        error = pthread_create(&tidcanibal[i], NULL, comer, &i);
        if (error){
            fprintf(stderr, "Error: %d: %s\n", error, strerror(error) );
            exit(-1);
        }
    }

    //espera a que los hilos terminen
    //no llega a ejecutarse ya que los canibales se encuentran en un bucle 
    // infinito esperando para comer
    for(i = 0; i < numcanibales; i++){
        error = pthread_join(tidcanibal[i], (void**)&salida);
        if (error){
            fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
            exit(-1);
        }
    }
    // No se llega a ejecutar ya que el cocinero espera en bucle infinito
    // a que llegue un comensal
    error = pthread_join(tidcocinero, NULL);
    if (error){
        fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
    }
}
