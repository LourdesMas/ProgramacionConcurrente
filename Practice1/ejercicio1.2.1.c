/**	$hilos
 Compilacion: cc -o hilos hilos.c -lpthread
 **/
//El standard en POSIX pthread es el más utilizado en entornos UNIX
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// define una macro cuyo valor no podrá ser cambiado durante la ejecución
// del codigo y que representa el número de hilos a utilizar
#define NUM_HILOS 5
//Variable global 
int I = 0;

// Función que devuelve puntero a void, apunta a una posición en memoria que
// posteriormente se puede castear al tipo a usar.
// Recibe un puntero a void que posteriormente en el codigo se castea a puntero
// a int en el printf, lo que es necesario para poder utilizar este puntero genérico
// Esta función lo que realiza es, ejecutar el for 50 veces, este for muestra 
// por pantalla el identificado del hilo el número de repetición en el bucle
// y el valor de I ademas de aumentar este último en 1 el numero de veces que se repite el bucle
// for de la función codigo_del_hilo, cada hilo que itere una vez sobre el for la aumentará en 1
// llegando hasta 250, 50 por cada hilo
// Cuando termina el hilo de ejecutar la función, devuelve un valor a traves de un puntero a void
// que está disponible para otro hilo en el mismo proceso que llame a pthread_join
void *codigo_del_hilo(void *id){
	int i;
	for (i = 0; i < 50; i++){
		printf("Hilo %d: i = %d, I = %d\n", *(int*)id, i, I++);
		}
	pthread_exit(id);
	}
	
// Funcion principal
int main(){
	
	// crea un vector de hilos llamado id de tipo pthread_t del tamaño del número de hilos
	// Tambien crea un vector que almacenará los id de los 5 hilos, de 1 a 5 
	pthread_t hilos[NUM_HILOS];
	int id[NUM_HILOS] = {1,2,3,4,5};
	int error;
	int *salida;
	//variable auxiliar para utilizar en los bucles for
	int h;
	// bucle for que llega hasta 5(numero de hilos deseado) utilizando 
	// la función pthread_create, esta función devuelve 0, pero si ha habido algun error
	// devuelve el número de error. Para cada hilo se llama a la función codigo_del_hilo pasándole un puntero
	// a una posición del vector id, de forma iterativa para cada llamada. 
	// En cambio si ha habido algun fallo, el if (error) condicionará a verdadero 
	// y se mostrará por pantalla el error y su código y posteriormente se cierra el proceso
	for (h = 0; h <NUM_HILOS; h++){
		error = pthread_create(& hilos[h],NULL, codigo_del_hilo,&id[h]);
		if (error){
			fprintf(stderr, "Error: %d: %s\n", error, strerror (error));
			exit(-1);
			}
		}
	// Utilizando un bucle for que se ejecuta tantas veces como hilos hay
	// se procede a unir los hilos usando pthread_join, se trata de una función
	// que espera a que el hilo termine y lo une, esta funcion recibe como argumentos 
	// el hilo a unir, y el valor de retorno que es dado por pthread_exit, la función devuelve
	// 0 si no ha habido ningún error y en caso contrario devuelve el código de error
	for (h = 0; h < NUM_HILOS; h++){
		error = pthread_join(hilos[h], (void**)&salida);
		// En caso de que la función pthread_join haya devuelto un error, éste se imprimirá
		// por pantalla
		if (error){
			fprintf(stderr,"Error: %d; %s\n", error, strerror (error));
			}
		// Y en caso de que el método no devuelva un error imprime el id
		// del hilo, que es devuelto por pthread_exit desde la función
		// codigo_del_hilo
		else{
			printf("Hilo %d terminado\n",*salida);
			}
		}
	}

// Ambos programas realizan la misma tarea, uno utilizando 5 procesos y otro usando
// 5 hilos.
// Las funciones pthread_exit ypthread_join son equivalentes a exit y wait en el uso de procesos 
// La salida de ambos programas es la misma, excepto para la variable global I,
// la razón de esta diferencia se explica a continuación: 
// el programa que trabaja con procesos utiliza 5 procesos distintos para repartirse
// las tareas, en cambio este programa utiliza 5 hilos, que realizan la concurrencia 
// dentro de un mismo proceso. El peso de ambas entidades es diferente, mientras que 
// los procesos son pesados (su estructura está en el núcleo y cada proceso que quiere 
// acceder a ella tiene que hacer una llamada al sistema). Los hilos, por contra, comparten
// la información del proceso, por lo que si un hilo mdifica una variable el resto de hilos 
// verá esa modificación.
// Por esta razón para el programa de procesos la variable global I para cada proceso tiene 
// el valor incrementa localmente, hasta que llega a 50 en todos los procesos. Y para el 
// programa de hilos el valor es compartido y cada hilo actualiza su valor, hasta llegar a 250
