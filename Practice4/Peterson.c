#include    <pthread.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <stdbool.h>

int I = 0;
int turn = 0;
bool states[] = {false, false};

void *codigo_del_hilo(void *id){
    //me
    int i = *(int*)id;
    //other thread
    int j = (i == 1)? 0:1;
    //iterations per thread
    int k;
    for (k = 0; k < 100; k ++){

        //entry protocol
        states[i] = true;
        turn = 1;
        while (states[j] && turn ==1){

        }
        //critical section
        I += 1;
        printf("En hilo: %d, I =%d\n", i, I);
        //exit protocol
        states[i] = false;

    }
    pthread_exit(id);

}
int main (){

    int h;
    pthread_t hilos[2];
    int id[2] = {1,2};
    int error;
    int *salida;

    for (h = 0; h < 2; h ++) {
        error = pthread_create(&hilos[h], NULL, codigo_del_hilo, &id[h]);
        if (error) {
            fprintf(stderr, "Error: %d, %s\n", error, strerror(error));
            exit (-1);
        } 
    }
    for (h = 0; h< 2; h++) {
        error =pthread_join(hilos[h],(void**)&salida);
        if (error) {
            fprintf(stderr, "Error: %d, %s\n", error, strerror(error));
        }else {
            printf ("Hilo %d terminado\n",*salida);
        }
    }



}