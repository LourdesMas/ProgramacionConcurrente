/***
$procesos
 ***/
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>

//variable global que contiene el número de procesos
#define NUM_PROCESOS 2
int I = 0;

void codigo_del_proceso(int id){
	int i;
	for (i = 0; i < 50; i ++)
		printf("%d\n",id);
	exit(id);
}

int main(){
	int p;
	int pid;
	int id [NUM_PROCESOS] = {1,2};
	int salida;
	
	for (p = 0; p < 2; p++){
		pid = fork();
		if(pid == -1){
			perror("Error al crear un proceso");
			exit( -1);
		}
		else if(pid == 0)
			codigo_del_proceso(id[p]);
		
	}
	for(p = 0; p < NUM_PROCESOS; p++){
		pid = wait(&salida);
		printf("proceso %d, con id %x terminado\n", pid, salida >> 8);
	}
}

