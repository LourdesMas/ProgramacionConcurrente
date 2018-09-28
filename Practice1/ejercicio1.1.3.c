/***
$procesos
***/

#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>

#define NUM_PROCESOS 2

void imprime_entero(int ent){
	printf("Imprime: %d\n", ent);
	exit(ent);
	}
	
int main(){
	int parametros [NUM_PROCESOS] = {1,2};
	int p;
	int pid;
	int salida;
	for (p = 0; p < NUM_PROCESOS; p++){
			pid = fork();
			if (pid == -1){
				perror("Error al crear el hijo\n");
				exit(-1);
				}
			else if(pid == 0){
				imprime_entero(parametros[p]);
				}
		}
	for (p = 0; p < NUM_PROCESOS; p++){
		pid = wait(&salida);
		printf("proceso %d, con id %x terminado\n", pid, salida >> 8);
		}
	
	}
