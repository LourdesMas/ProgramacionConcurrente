#include "comida.h"
#include <unistd.h> //para hacer el sleep

/*********************************************************
Los beneficios del uso de monitores sobre el de semáforos es el de
la relegación de código, dejamos que sea el monitor el que gestione
como se controla el uso de los recursos por los procesos, desacoplando 
el código y haciendo que sea más fácil poder realizar futuras 
modificaciones del código o añadir nuevas secciones criticas

***************************************************************/




//variable global que indica el numero de comidas que hara cada filósofo
#define NUM_COMIDAS 100

//estructura para identificar a un filósofo, con su numero de filósofo
//y con un puntero al monitor comida 
typedef struct {
    int num_filosofo;
    comida_t *comida;

}filosofos_t;

//un pequeño retardo para ver mejor los resultados por pantalla
void retardo(){
    struct timespec tim, tim2;
    tim.tv_sec = 1;//segundos
    tim.tv_nsec = 000000; //nanosegundos
    nanosleep(&tim,&tim2);
}

//función para indicar qne el filósofo está pensando
void think(int num_filosofo){
    printf("El filosofo %d piensa\n", num_filosofo);
}
//función para indicar qne el filósofo come
void eat(int num_filosofo){
    printf("El filosofo %d come\n", num_filosofo);
    retardo();
    printf("El filosofo %d para de comer\n", num_filosofo);
}

//código del proceso, recibe el filósofo que representa el proceso,
// (nuestro monitor)
void *cod_filosofo(void *arg){
    
    filosofos_t filosofo = *(filosofos_t*) arg;
    //para todas las veces que el filósofo va a comer
    for (int i = 0; i < NUM_COMIDAS; i++){
        //primero piensa
        think(filosofo.num_filosofo);
        //después coge los palillos
        pick(filosofo.comida,filosofo.num_filosofo);
        //come
        eat(filosofo.num_filosofo);
        //deja los palillos
        release(filosofo.comida,filosofo.num_filosofo);
    }
    pthread_exit(NULL);
    return NULL;

}

int main (){
    //creamos nuestros filósofos
    filosofos_t filosofos[FILOSOFOS];
    //creamos una comida
    comida_t comida = comida_factory();
    //
    pthread_t filosofosthread[FILOSOFOS];
    int error, i;

    //creación de los hilos
    for (i = 0; i < FILOSOFOS; i++){
        filosofos[i].num_filosofo = i;
        filosofos[i].comida = &comida;
        error = pthread_create(&filosofosthread[i],NULL,cod_filosofo, (void*)&filosofos[i]);
        gestionarError(error);
    }

    //espera a que los hilos terminen
    //Join de los hilos
    for (i = 0;  i < FILOSOFOS; i++){
        error = pthread_join(filosofosthread[i], NULL);
        gestionarError(error);
    }

    return 1;

}
