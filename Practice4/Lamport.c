#include    <pthread.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <stdbool.h>
int size = 2;
int I = 0;
int number[] = {0, 0};
bool states[] = {false, false};


int maxValue(){
    int currentMax = 0;
    for (int i = 0; i < size; i++){
        if (number[i] > currentMax){
            currentMax = number[i];
        }
    }
    return currentMax;
}

void *codigo_del_hilo(void *id){
    //me
    int i = *(int*)id;


    int k;
    for (k = 0; k < 100; k ++){

        //entry protocol
        states[i] = true;
        number[i] = 1 + maxValue();
        states[i] = false;
        for (int j = 0; j < size; j++){
            while(states[j] == true);
            while (number[j] > 0 && 
                    (number[j] < number[i] || 
                        (number[j] == number[i] && j < i)));
            
        }

        //critical section
        I += 1;
        printf("En hilo: %d, I =%d\n", i, I);
        //exit protocol
        number[i] = 0;

    }
    pthread_exit(id);

}
int main (){

    int h;
    pthread_t hilos[2];
    int id[2] = {0,1};
    int error;
    int *salida;

    for (h = 0; h < size; h ++) {
        error = pthread_create(&hilos[h], NULL, codigo_del_hilo, &id[h]);
        if (error) {
            fprintf(stderr, "Error: %d, %s\n", error, strerror(error));
            exit (-1);
        } 
    }
    for (h = 0; h< size; h++) {
        error =pthread_join(hilos[h],(void**)&salida);
        if (error) {
            fprintf(stderr, "Error: %d, %s\n", error, strerror(error));
        }else {
            printf ("Hilo %d terminado\n",*salida);
        }
    }



}