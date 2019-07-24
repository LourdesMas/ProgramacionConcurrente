#include "comida.h"



//factoria para inicializar una estructura comida_t por defecto
//inicializamos el cerrojo, las variables condiciones para indicar
//si cada filósofo puede comer y el vector de palillos
// inicializado a 2 en todos sus elementos, ya que todos los palillos
//empiezan estando disponibles para todos los filosofos
comida_t comida_factory(){
    comida_t comida;
    pthread_mutex_init(&comida.lock, NULL);
    for (int i = 0; i < FILOSOFOS; i++){
        comida.forks[i] = 2;
         pthread_cond_init(&comida.canEat[i], NULL);

    }
    return comida;
}
//devuelve el palillo a la izquierda del número de filósofo pasado
//por parametro, en caso de ser el primer filósofo, debera volver
// el último palillo, al haber tantos filosofos como palillos,
// por eso se devuelve el resultado modulo tamaño
int left(int i){
    return (i + FILOSOFOS - 1) % FILOSOFOS;
}
//devuelve el palillo a la derecha del número de filósofo pasado
//por parametro, en caso de ser el último filósofo, debera volver
// el primer palillo, ya que hay tantos filosofos como palillos
//se devuelve el resultado modulo tamaño
int right(int i){
    return (i + 1) % FILOSOFOS;
}
//
void pick(comida_t *comida, int i){
    int error;
    //bloqueamos el acceso a la sección crítica con el cerrojo
    //para poder actualizar el estado de los palillos 
    error = pthread_mutex_lock(&comida->lock);
    //gestionamos el posible error
    gestionarError(error);
    //esperamos a que el filósofo tenga dos palillos
    while(comida->forks[i] != 2){
        //la variable condición nos tiene en espera 
        //hasta que los dos palillos del filósofo estén disponibles
        error = pthread_cond_wait(&comida->canEat[i],&comida->lock);
        //gestionamos el posible error
        gestionarError(error);
    }
    //vamos a coger dos palillos, uno a la izquierda, y otro a la derecha
    // para ello los filosofos que tenemos a los lados, tendrán un palillo 
    //menos cada uno
    comida->forks[left(i)] --;
    comida->forks[right(i)]--;
    printf("El filósofo %d coge los palillos\n", i);
    //desbloqueamos el acceso a la sección crítica con el cerrojo
    error = pthread_mutex_unlock(&comida->lock);
    //gestionamos el posible error
    gestionarError(error);

}
void release(comida_t *comida, int i){
    int error;
    //bloqueamos el acceso a la seccin crítica con el cerrojo
    error = pthread_mutex_lock(&comida->lock);
    //gestionamos el posible error
    gestionarError(error);
    printf("El filósofo %d deja los palillos\n", i);
    //los filosofos de la izquierda y de la derecha tendran un palillo mas
    //cada uno
    comida->forks[left(i)]++;
    comida->forks[right(i)]++;
    //si el filósofo de la izquierda tiene dos palillos ya disponibles
    //liberamos al proceso en la cola de la variable de condición 
    //para que este filósofo pueda comer
    if (comida->forks[left(i)] == 2){
        error = pthread_cond_signal(&comida->canEat[left(i)]);
        //gestionamos el posible error
        gestionarError(error);
    }
    //si el filósofo de la izquierda tiene dos palillos ya disponibles
    //liberamos al proceso en la cola de la variable de condición 
    //para que este filósofo pueda comer
    if (comida->forks[right(i)] == 2){
        //gestionamos el posible error
        error = pthread_cond_signal(&comida->canEat[right(i)]);
    }
    //desbloqueamos el acceso a la sección crítica con el cerrojo
    error = pthread_mutex_unlock(&comida->lock);
    //gestionamos el posible error
    gestionarError(error);

}


//mÉtodo para gestionar las salida de error
void gestionarError(int error){
    if (error){
        fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
        exit(-1);
    }
}
