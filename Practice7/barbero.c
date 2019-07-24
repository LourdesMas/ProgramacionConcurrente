#include <pthread.h> //para usar hilos
#include <semaphore.h> // para usar los semáforos
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // para sacar errores por pantalla
#include <unistd.h> //para hacer el sleep

#define clientes 20 //número máximo de clientes, usado para generalizar y facilitar el uso de la cantidad de hilos de cliente
#define barberos 3 //número máximo de barberos, usado para generalizar y facilitar el uso de la cantidad de hilos de barbero

//unnamed POSIX semaphores, declaración

sem_t max_capacidad;//capacidad máxima de cliente que tiene la barbería

sem_t sofa;//controla el uso del sofá por los clientes. inicializado a 4

sem_t silla_barbero;// controla las sillas que usan los barberos para cortar. inicializado a 3

sem_t cliente_listo;//para indicar que el cliente ya esta sentado en la silla, inicializado a 0

sem_t terminado;//indica que el corte de pelo ha terminado, inicializado a 0

sem_t dejar_silla_barbero;//se usa cuando el cliente abandona la silla después del corte de pelo, inicializado a 0

sem_t pago;//controla el pago de los clientes, inicializado a 0

sem_t recibo;//entrega al cliente de un recibo de pago, indica que se ha realizado el pago, inicializado a 0

sem_t coord;//controla la ocupacion de los barberos, tanto para cortar como para cobrar, inicializado a 3.

//un pequeño retardo para ver mejor los resultados por pantalla
void retardo(){
    sleep(1);
}
//MÉTODOS DE SALIDA LLAMADOS POR thread_barbero
void cortar_pelo(int id){
    printf("El peluquero %d está cortando el pelo\n", id);
    retardo();
}
// código de los hilos de barbero
void *thread_barbero(void *idd){
    int id = *(int*) idd;
    //bucle infinito, el barbero siempre estará activo 
    while(1){

        //el barbero espera a que haya un cliente listo, al estar
        //cliente_listo inicializado a 0, tendrá que esperar a que un cliente haga un post
        // a cliente_listo, para que el barbero pueda continuar su ejecución
        sem_wait(&cliente_listo);
        //a continuación se esperara a que haya un peluquero disponible, al
        //estar inicializado a 3 los 3 primeros podrán automaticamente continuar, después
        //habrá que esperar a que se haga post, siempre habrá como máximo 3 peluqueros a la vez
        //pudiendo ejecutar la zona que va a continuación
        sem_wait(&coord);
        cortar_pelo(id);
        //el peluquero vuelve a estar disponible, ha terminado de cortar el pelo
        sem_post(&coord);
        //indica que el corte ha terminado y el código del cliente, en el que se hace wait de terminado puede continuar
        sem_post(&terminado);
        //el cliente posteriormente a recibir el post de terminado, hará un post de 
        //dejar silla barbero, y asi el barbero podrá continuar, hasta que el cliente
        //no haga post, no se podrá continuar ya que el semáforo esta inicializado a 0
        sem_wait(&dejar_silla_barbero);
        //una de las sillas de barbero queda libre, este post se hace ya que el cliente 
        //se ha levantado, y cualquier cliente que esperara que quedará libre la silla 
        //podrá sentarse
        sem_post(&silla_barbero);

    }
    pthread_exit(NULL);

}
//MÉTODOS DE SALIDA LLAMADOS POR thread_cajero
void cobrar(){
    printf("El cajero realiza el cobro\n");
    retardo();
}
// código de los hilos de cajero
void *thread_cajero(void *id){

    //bucle infinito el cajero siempre estara activo
    while(1){
        //espera a que haya un cliente preparado para pagar, al estar inicializado a 0
        //tendrá que esperar a que se haga un post, que se hará desde el cliente, después
        //de dejar el asiento de la barbería
        sem_wait(&pago);
        //espera a que haya un peluquero disponible para poder canjear el pago
        sem_wait(&coord);

        cobrar();
        //el peluquero ocupado vuelve a estar disponible
        sem_post(&coord);
        //el cliente que espera en su código para poder recibir el recibo ya puede hacerlo
        sem_post(&recibo);
    }
    pthread_exit(NULL);

}
//MÉTODOS DE SALIDA LLAMADOS POR thread_cliente
void salir_tienda(int id){
    printf("El cliente %d sale de la tienda\n", id);
    retardo();
}
void pagar(int id){
    printf("El cliente %d paga al barbero\n", id);
    retardo();
}
void levantarse_silla_barbero(int id){
    printf("El cliente se levanta de la silla del barbero\n", id);
    retardo();
}
void sentarse_silla_barbero(int id){
    printf("El cliente %d se sienta en la silla del barbero\n", id);
    retardo();
}
void levantarse_sofa(int id){
    printf("El cliente %d se levanta del sofa\n", id);
    retardo();
}

void sentarse_sofa(int id){
    printf("El cliente %d se sienta en el sofa\n", id);
    retardo();
}
void entrar_barberia(int id){
    printf("El cliente %d entra en la barberia\n", id);
    retardo();
}

// código de los hilos de cliente
void *thread_cliente(void *idd){
    int id = *(int*) idd;
    //bucle infinito el cliente siempre vuelve a querer ser atendido
    while(1){

        //espera hasta que quepa un cliente en la barbería, el máximo es 20,
        //al empezar inicializado a 20, los 20 primeros podrán entrar seguidos
        //cada uno de los siguientes debera esperar a que se haga un post
        //entrarán no superando nunca 20 simultaneos
        sem_wait(&max_capacidad);
        entrar_barberia(id);
        //una vez ha entrado en la barbería, espera a que haya sitio en el sofá
        //para eso se usa el semáforo sofa, inicializado a 4, los 4 primeros podrán
        //entrar seguidos, pero el resto debera esperar a que un cliente que pase a usar
        //la silla haga post, habrán hasta 4 a la vez.
        sem_wait(&sofa);
        sentarse_sofa(id);
        //el siguiente paso es esperar que una de las sillas de barbero
        //esté disponible, la situación es la misma que cuando espera el sofa,
        //pero el semáforo en lugar de tener capacidad de 4, la tiene de 3
        //en caso de que este el semáforo a 0, tendrá que esperar a que en el código
        //del barbero se haga un post, o sea, una silla quede disponible.
        sem_wait(&silla_barbero);
        levantarse_sofa(id);
        //al poder sentarse en la silla del barbero, el cliente deja libre el sofá,
        //por tanto, hace post para que otro que esté esperando pueda ocuparlo
        sem_post(&sofa);

        sentarse_silla_barbero(id);
        //se avisa de que hay un cliente listo, al estar inicializado a 0, el barbero que espera
        // ya puede ejecutar su código dentro del while de su código
        sem_post(&cliente_listo);
        //espera a que el semáforo terminado onicializado a 0 se ponga a 1
        // esto será cuando el barbero haga el post al haber terminado de cortar el pelo
        //del cliente
        sem_wait(&terminado);
        levantarse_silla_barbero(id);
        //pone el semáforo dejar_silla_barbero a 1, indicandole al barbero en su código
        //que ya no esta sentado, para que el barbero libere una silla, haciendo post en su código
        //de esta forma otro cliente podrá usarla
        sem_post(&dejar_silla_barbero);

        pagar(id);
        //pone el semáforo de pago a 1, solo se puede realizar un pago a la vez, y 
        // el cajero que espera pago puede cobrar
        sem_post(&pago);
        //después de hacer el post del pago, el cliente espera a que el cajero
        //haga un post de recibo, inicializado a 0, para ponerlo a 1 y por tanto
        //el cliente pueda continuar (el wait volverá a poner el semáforo a 0 y de
        //esta forma el siguiente cliente, deberá volver a esperar a que un cajero 
        //haga post otra vez)
        sem_wait(&recibo);
        salir_tienda(id);
        //la capacidad se incrementa en 1 y si hubiera un cliente esperando para entrar
        //podráa hacerlo y por tanto continuar la ejecución en el while del cliente
        // para volver a repetir todo el proceso por el que debe pasar un cliente.
        sem_post(&max_capacidad);
    }
    pthread_exit(NULL);
}

//mÉtodo para gestionar las salida de error
void gestionarError(int error){
    if (error){
        fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
        exit(-1);
    }
}



int main(int argc, char *argv[]){

    int error;

    //declaramos los hilos, 
    pthread_t barbero[barberos];
    pthread_t cliente[clientes];
    pthread_t cajero;

    /*********************************************************
    Aclaración sobre los semáforos
    Aquellos cuyo contador se inicializa a cero se utilizan para pasar al siguiente paso
    en la atención, por ejemmplo, pasar de esperar a ser atendido en una sección, a utilizarla.
    Estos semáforos alternarán entre 0 y 1 su valor. al hacerse un post, se permitirá pasar a la 
    siguiente seccion, y la seccion, que hace wait, lo pondrá de nuevo a 0.

    Aquellos semáforos que se inicializan a 20, 4 y 3,
    max_capacidad, sofa y silla_barbero, se emplean como contadores para indicar 
    la cantidad de clientes que pueden pasar a cada sección simultaneamente, 20, 
    el mayor número,  para indicar los que caben en la tienda, 4 en el sofa de espera
    y 3 en la silla del barbero. Ademas coord se utilizará para que no pueda haber más de tres 
    barberos simultaneamente.
    Estos semáforos inicializados a un valor positivo, permitirán que entren en la sección hasta el mismo
    número de ejecuciones paralelas al que esta inicializado simultaneamente. Se podrá avanzar iterando tras cierta cantidad de 
    waits, hasta que el valor del semáforo llegue a 0, entonces habrá que esperar a que se haga un post para 
    que otra ejecución paralela pueda entrar.

    *********************************************************/

    //INICIALIZACIÓN DE SEMÁFOROS, primer valor a 0, inidica que son hilos,
    //el segundo valor indica el número al que se inicializan los semáforos.

    //se inicializa a 20, ya que esta será la capacidad máxima, los primeros 20 
    //sem_waits que se produzcan podrán entrar, pero al llegar al 20, el valor del semáforo
    //sera 0 y habrá que esperar un post, de esta manera, siempre habrá 20 clientes como 
    //máximo
    sem_init(&max_capacidad,0,20);
    //sofa con capacidad para 4 clientes, su funcionamiento es como el explicado en max_capacidad
    //y no se superara la cantidad de 4 clientes usando el sofa
    sem_init(&sofa,0,4);
    //semáforo que representa la silla del barbero, inicializada a 3, porque hay tres sillas que pueden
    //ser usadas simultaneamente.
    sem_init(&silla_barbero,0, 3);
    //semáforo para indicar que el cliente esta listo para ser atendido en alguna sección, inicializado a 0
    //se pondrá a 1 para indicar que está listo
    sem_init(&cliente_listo,0,0);
    //semáforo para indicar que el peluquero ha terminado de cortar el pelo inicializado a 0
    //se pondrá a 1 para indicar que ha terminado
    sem_init(&terminado,0,0);
    //semáforo para indicar que el cliente se levanta de la silla del barbero inicializado a 0
    //se pondrá a 1 para indicar que se ha levantado
    sem_init(&dejar_silla_barbero,0,0);
    //semáforo para indicar que el cajero cobra el pago inicializado a 0
    //se pondrá a 1 para indicar que el cajero puede realizar este cobro
    sem_init(&pago,0,0);
    //semáforo para indicar que el cliente recibe el recibo inicializado a 0
    //se pondrá a 1 para indicar que cliente ha recibido el recibo y ya puede terminar
    sem_init(&recibo,0,0);
    //se usa para controlar la ocupación de los 3 barberos
    sem_init(&coord,0,3);



    //CREACION DE LOS HILOS
    //creación del hilo del cajero
    error = pthread_create(&cajero, NULL, thread_cajero, NULL);
    gestionarError(error);


    int i;
    //creacion de los hilos de barbero
    for (i = 0; i < barberos;i++){
        error = pthread_create(&barbero[i], NULL, thread_barbero, &i);
        gestionarError(error);
    }
    //creacion de los hilos de cliente
    for(i = 0; i < clientes ;i++){
        error = pthread_create(&cliente[i], NULL, thread_cliente, &i);
        gestionarError(error);
    }



    //espera a que los hilos terminen
    //Join de los hilos
    error = pthread_join(cajero, NULL);
    gestionarError(error);
    for (i = 0; i < barberos;i++){
        error = pthread_join(barbero[i], NULL);
        gestionarError(error);
    }
    
    for(i = 0; i < clientes ;i++){
        error = pthread_join(cliente[i], NULL);
        gestionarError(error);
    }
}