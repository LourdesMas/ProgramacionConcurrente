#include    <stdio.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <stdlib.h>
#include    <string.h>

#define NUM_HILOS 5  
int I = 0; 
int iteraciones;  // numero de interaciones


void protocolo_de_entrada() {
    asm(
        ".data\n\t" /* Declaro la variable estatica lock (solo vale para hilos)*/
        "lock:\n\t"
        ".byte 0\n\t"  /* la inicializo a 0 (seccion critica libre) */
        ".text\n\t" /* comienzan las instrucciones */
        "_protocolo_de_entrada:\n\t"   /* etiqueta al principio de la espera ocupada */
        "movb $1, %al\n\t"   /* muevo 1 al registro AL */
        "xchgb (lock),%al\n\t"  /* intercambio el valor de lock con 1 de manera atomica*/
        "cmp $0, %al\n\t"    /* comparo lo que habia en lock con 0 */
        "jne _protocolo_de_entrada\n\t"  /* si lock era 0, la seccion critica estaba libre */ 
                      /* y salgo de la espera ocupada, ahora lock = 1,  */ 
                      /* si lock era 1, sigue siendo 1 y vuelvo a comprobar */
    );
}

void protocolo_de_salida() {
    asm("movb $0, (lock)");    // seccion critica libre
}


void region_critica() {
  I = I + 1;
  printf("La variable compartida vale %d\n",I);
}

void resto_proceso() {

}

static void* f(void* p) {
    int i;
    for(i = 0; i < iteraciones; i++) 
    {
        protocolo_de_entrada();
        region_critica();
        protocolo_de_salida();
        resto_proceso();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int error;
    int h;

    if(argc != 3){
    fprintf(stderr, "Uso: %s <iteraciones> <num_hilos>\n",argv[0]);
    return 0;
    }
    iteraciones = atoi(argv[1]);
    int num_hilos = atoi(argv[2]);
    pthread_t hilos[num_hilos];


    for(h = 0; h < num_hilos; h++){
      error = pthread_create( &hilos[h], NULL, f, NULL);
      if (error){
        fprintf (stderr, "Error: %d: %s, problemas creando el hilo %d\n",
                 error, strerror (error), h);
        exit(-1);
      }
    }
    for(h =0; h < num_hilos; h++){
      error = pthread_join(hilos[h], NULL);
      if (error)
         fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
    }

    printf("La variable compartida vale %d y tenía que valer %d\n",I, num_hilos*iteraciones);
    return 0;
}
