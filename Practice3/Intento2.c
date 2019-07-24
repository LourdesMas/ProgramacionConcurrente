#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int I = 0;
bool states[] = {false, false};

void *codigo_del_hilo(void *id){
    int i = *(int*)id;
    int j = (i == 1)? 0:1;
    int k;
    for(k = 0; k < 100; k ++){
        //protocolo de entrada
        while(states[j]);
        states[i] = true;
        //seccion critica
        I = (I + 1) % 10;
        printf("En hilo %d, I=%d\n", i,I);
        //protocolo de salida
        states[i] = false;
        //resto
    }
    pthread_exit (id);

}
int main(){

    int h;
    pthread_t hilos[2];
    int id[2]={1,2};
    int error;
    int *salida;

    for(h=0; h<2; h++){
        error = pthread_create(&hilos[h], NULL,codigo_del_hilo, &id[h]);
        if (error){
            fprintf(stderr, "Error: %d: %s\n", error, strerror (error));
            exit(-1);
        }
    }
    for(h = 0; h < 2; h++){
        error= pthread_join(hilos[h],(void**)&salida);
        if (error)
            fprintf(stderr, "Error: %d: %s\n", error, strerror(error) );
        else
            printf("Hilo %d terminado\n",*salida);
    }
}