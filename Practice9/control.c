#include "control.h"

//factoria para inicializar una estructura de pistas_t por defecto, devuelve las pistas por defecto
pistas_t pistas_factory(){
    pistas_t pistas;
    //inicializamos el cerrojo
    pthread_mutex_init(&pistas.lock, NULL);
    for (int i = 0; i < PISTAS; i++){
        //inicializamos las variables, ningún avión ha solicitado aterrizar
        pistas.aterriza[i] = 0;
        //ningún avión está usando las pistas
        pistas.usandoLaPista[i] = 0;
        //inicializamos la variable de condición
        pthread_cond_init(&pistas.puedeAterrizar[i], NULL);
        pthread_cond_init(&pistas.puedeDespegar[i], NULL);
    }
    return pistas;
}

//devuelve un valor que representa a un número de pista
int darPistaDespegue(){
    return rand() % PISTAS;
}

//en la pista con el id pasado por parametro "p" despega un avión "i"
void despega(pistas_t *pista, int i, int p){
    int error;
    //bloqueamos el acceso a la sc con el cerrojo para poder actualizar
    //el estado de la pista
    error = pthread_mutex_lock(&pista->lock);
    gestionarError(error);
    
    //esperamos a que la pista tenga espacio, osea menos de tres aviones despegando y ninguno pendiente de
    //aterrizar
    while(pista->usandoLaPista[p] >2 || pista->aterriza[p] > 0){
        error = pthread_cond_wait(&pista->puedeDespegar[p],&pista->lock);

        gestionarError(error);
    }

    //vamos a ocupar la pista y despegar
    pista->usandoLaPista[p] ++;
    printf("Avion %d despega en pista %d\n", i, p);
    error = pthread_mutex_unlock(&pista->lock);
    gestionarError(error);
}
//en la pista con el id pasado por parametro "p" termina de despegar el avion "i"
void terminaDeDespegar(pistas_t *pista, int i, int p){
    int error;
    //bloqueamos el acceso a la sc
    error = pthread_mutex_lock(&pista->lock);
    //gestionamos el posible error
    gestionarError(error);
    //liberamos la plaza en la pista
    pista->usandoLaPista[p] --;
    printf("Avion %d termina de despegar en pista %d\n", i, p);
    
    //liberamos al proceso que estuviera esperando
    if(pista->aterriza[p]>0){
        //si hay un avion que quiere aterrizar 
        if (pista->usandoLaPista[p] == 0){
            //si la pista ya esta libre
            error = pthread_cond_signal(&pista->puedeAterrizar[p]);
            gestionarError(error);
        }
    }else{
        //sino permite que se pueda despegar
        error = pthread_cond_signal(&pista->puedeDespegar[p]);
        gestionarError(error);
    }
    

    error = pthread_mutex_unlock(&pista->lock);
    gestionarError(error);
}

//aterriza un avión "i", la pista se decide según cual tenga más espacio libre
void aterriza(pistas_t *pista, int i){
    int error;
    //bloqueamos el acceso a la sc con el cerrojo para poder actualizar
    //el estado de la pista
    error = pthread_mutex_lock(&pista->lock);
    gestionarError(error);

    //elegimos la pista con mas espacio libre
     int p = (pista->usandoLaPista[0] < pista->usandoLaPista[1]) ? 0:1;
    //solicita aterrizar, esta variable con valor mayor a 0 bloqueará a otros aviones que quieran despegar en esa pista
    pista->aterriza[p] += 1;
    
    //esperamos a que la pista esté vacia, solo un avión aterriza a la vez
    while(pista->usandoLaPista[p] >0 ){
        error = pthread_cond_wait(&pista->puedeAterrizar[p],&pista->lock);
        gestionarError(error);
    }
    //vamos a ocupar la pista y aterrizar
    pista->usandoLaPista[p] ++;
    printf("Avion %d aterrizando en pista %d\n", i, p);
    error = pthread_mutex_unlock(&pista->lock);
    gestionarError(error);

    //termina de aterrizar, protocolo de salida
    //bloqueamos de nuevo el acceso a la sc
    error = pthread_mutex_lock(&pista->lock);
    //gestionamos el posible error
    gestionarError(error);
    //liberamos la plaza en la pista
    pista->usandoLaPista[p] --;
    printf("Avion %d termina de aterrizar en pista %d\n", i, p);
    pista->aterriza[p] -= 1;
    
    
    //si no hay otro avión esperando aterrizar en esta pista, permitimos que los aviones puedan despegar
    if(pista->aterriza[p] == 0){
        error = pthread_cond_signal(&pista->puedeDespegar[p]);
        gestionarError(error);
    }else{
        error = pthread_cond_signal(&pista->puedeAterrizar[p]);
        gestionarError(error);
    }
    
    error = pthread_mutex_unlock(&pista->lock);
    gestionarError(error);

}

//metodo para controlar los errores, salida por consola del problema y termina la ejecución
void gestionarError(int error){
    if (error){
        fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
        exit(-1);
    }
}
