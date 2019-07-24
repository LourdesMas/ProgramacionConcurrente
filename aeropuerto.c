#include "control.h"
#include <unistd.h> //para hacer el sleep

#define NUM_AVIONES 200

//estructura para identificar el avión, con su numero 
//y con un puntero al monitor control
typedef struct {
    //identificador del avión
    int num_avion;
    //puntero a las pistas
    pistas_t *pistas;
}aviones_t;

//código de proceso para los aviones que despegan
void *cod_avion_despega(void *arg){
    aviones_t avion = *(aviones_t*) arg;
    //otorga una pista de despegue de forma aleatoria
    int pistaDada = darPistaDespegue();
    //protocolos de despegue
    despega(avion.pistas, avion.num_avion, pistaDada);
    //fin de despegue
    terminaDeDespegar(avion.pistas, avion.num_avion, pistaDada);
    pthread_exit(NULL);
    return NULL;
}

//código de proceso para los aviones que aterrizan
void *cod_avion_aterriza(void *arg){
    aviones_t avion = *(aviones_t*) arg;
    //protocolos de aterrizaje para el avión
    aterriza(avion.pistas, avion.num_avion);
    pthread_exit(NULL);
    return NULL;
}

int main(){
    //generamos la semilla con la hora actual, 
    //se usara para dar pistas aleatorias en el despegue
    srand(time(0));
    //creamos nuestros aviones
    aviones_t aviones[NUM_AVIONES];
    pistas_t pistas = pistas_factory();

    pthread_t avionesthread[NUM_AVIONES];
    int i, error, AoD;
    //creacion de los hilos, se elige que aviones aterrizan o despegan de forma aleatoria

    for (i = 0; i < NUM_AVIONES; i++ ){
        aviones[i].num_avion = i;
        aviones[i].pistas = &pistas;
        //Aterriza o Despega, 0->Aterriza, 1->Despega
        AoD = rand() % 2;
        if (AoD == 0){
            error = pthread_create(&avionesthread[i],NULL,cod_avion_aterriza, (void*)&aviones[i]);
            gestionarError(error);
        }
        else{
            error = pthread_create(&avionesthread[i],NULL,cod_avion_despega, (void*)&aviones[i]);
            gestionarError(error);
        }
    }
    
    //espera a que los hilos terminen
    //join de los hilos
    for (i = 0; i < NUM_AVIONES; i++){
        error = pthread_join(avionesthread[i], NULL);
        gestionarError(error);
    }
    return 1;
}