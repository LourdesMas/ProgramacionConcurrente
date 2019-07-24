#ifndef _COMIDA_H_
#define _COMIDA_H_
//#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "stdio.h"

//El número de filosofos y las plazas en la mesa
#define FILOSOFOS 10
    // estructura que usaremos para controlar el monitor
    typedef struct comida{
        //representa el número de palillos que tiene disponible
        //cada filósofo, de 0 a 2.
        int forks[FILOSOFOS];
        //semaforo de acceso a la seccion critica, para poder liberar y coger los palillos
        pthread_mutex_t lock;
        //variables condicion, de pthread.h, una condicion por filósofo
        //
        pthread_cond_t canEat[FILOSOFOS];
    } comida_t;

    //factoria para inicializar una estructura comida_t por defecto
    comida_t comida_factory();
    //devuelve el palillo a la izquierda del número de filósofo pasado
    //por parametro
    int left(int i);
    //devuelve el palillo a la derecha del número de filósofo pasado
    //por parametro
    int right(int i);
    //el filósofo con el id pasado por parametro "i" coge los palillos
    //se actualiza la estructura de nuestro monitor, los palillos de la izquierda
    // y derecha del filósofo dejaran de estar disponibles
    void pick(comida_t *comida, int i);
    //el filósofo con el id pasado por parametro "i" deja los palillos
    //se actualiza la estructura de nuestro monitor, los palillos de la izquierda
    // y derecha del filósofo volverán de estar disponibles
    void release(comida_t *comida, int i);
    //metodo para controlar los errores, salida por consola del problema y termina ejecución
    void gestionarError(int error);

#endif