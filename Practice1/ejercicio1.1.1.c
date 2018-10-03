/***
$procesos
***/
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>

// define una macro cuyo valor no podrá ser cambiado durante la ejecución
// del codigo y que representa el número de procesos a utilizar
#define NUM_PROCESOS 5

// Variable global, el valor no será compartido entre los procesos
int I = 0;

//metodo al que llama el hijo
void codigo_del_proceso (int id){
	int i;
	//bucle que se repite 50 veces, imprimiendo por pantalla el identificador 
	// que se le ha dado al proceso el número de iteración del buche y el valor 
	// de la variable I, que, al no ser compartida por los procesos, llegará hasta 50
	for(i = 0; i < 50; i++)
		printf("Proceso%d: i = %d, I = %d\n", id, i, I++);
	// una vez se termina el bucle el proceso hijo termina y devuelve su id 
	// al padre
	exit(id); // el id se almacena en los bits 8 al 15 antes de devolverlo al padre
}


int main(){
	int p;
	// vector que se utilizará para numerar los procesos de 1 a 5
	int id [NUM_PROCESOS] = {1,2,3,4,5};
	int pid;
	int salida;
	//bucle que se recorre 5 veces. tantas como el valor de la variable NUM_PROCESOS
	for(p = 0; p < NUM_PROCESOS; p++){
		// llamo a fork, creando un nuevo proceso. En unix todos los procesos 
		// excepto el primero se crean con una llamada a fork. Fork duplica todo
		// menos el BCP, y devuelve al padre el PID del hijo. almacenado en la variable pid
		// el hijo sigue la ejecución por donde iba el padre.
		pid = fork ();
		// en caso de que el pid sea -1 se ha producido un error al crear
		//el proceso hijo y el programa sale con codigo de error
		if(pid == -1){
			perror("Error al crear un proceso: ");
			exit(-1);
		}
		//En cambio si se ha hecho fork correctamente, se llama a la función
		//codigo_del_proceso (int id) que recibe el valor en la posición p 
		//del vector id, esto quiere decir, para el primer fork que hagamos
		//p será 0 y id[0] será 1, para el segundo p será 1 y id[1] será 2
		// y así sucesivamente hasta que el contador p llegue a 4 y el valor 
		//en la última posición del vector sea 5, con esto conseguimos 
		//numerar los procesos del 1 al 5 
		else if (pid == 0) // Codigo del hijo
			codigo_del_proceso(id[p]);
	}
	// Codigo del padre
	// Una vez que el padre ha dejado de crear procesos en el bucle for
	// se procede a terminar el proceso , para ello se usa la función wait
	// para esperar a que terminen los hijos su ejecución, 
	// esta función recibe un puntero a int en el que se almacena el valor devuelto por la 
	// funcion exit, en este caso el id del hijo
	for (p = 0; p < NUM_PROCESOS; p++){
		pid = wait (&salida);
		printf("Proceso %d con id = %x terminado*************************************\n", pid, salida >> 8);
	}
}
