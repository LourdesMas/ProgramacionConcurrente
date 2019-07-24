#ifndef _CONTROL_H
#define _CONTROL_H

#include <pthread.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

//Número de pistas
#define PISTAS 2

typedef struct pistas{
    //número de aviones que han solicitado aterrizar en cada pista
    int aterriza[PISTAS];
    //numero de aviones usando la pista en cada pista, será un valor entre 0 y 3
    int usandoLaPista[PISTAS] ;
    //semaforo de acceso a la sección crítica para poder modificar el estado de las pistas
    pthread_mutex_t lock;
    //Variable de condición de pthread que se usará para dar acceso a un avión a la pista
    pthread_cond_t puedeAterrizar[PISTAS];
    pthread_cond_t puedeDespegar[PISTAS];

}pistas_t;

//factoria para inicializar una estructura pistas_t
pistas_t pistas_factory();
//otorga una pista al avion pasado por parametro de manera aleatoria
int darPistaDespegue();
//en la pista con el id pasado por parametro "p" despega un avión "i"
void despega(pistas_t *pista, int i, int p);
//en la pista con el id pasado por parametro "p" termina de despegar el avion "i"
void terminaDeDespegar(pistas_t *pista, int i, int p);
//en la pista pasada por parametro "i" aterriza un avión
void aterriza(pistas_t *pista, int i);
//metodo para controlar los errores, salida por consola del problema y termina la ejecución
void gestionarError(int error);

#endif