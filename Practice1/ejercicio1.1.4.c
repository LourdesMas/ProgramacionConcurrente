/***
 $procesos
 * ***/
 
 #include<sys/types.h>
 #include<stdlib.h>
 #include<stdio.h>
 #include<sys/wait.h>
 
 #define NUM_PROCESOS 3
 
 void imprimir(char car){
	 unsigned int i;
	 for ( i = 0; i < 5; i++){
		 printf("%c",car);
		 }
	 printf("\n");
	 exit(i);
	 
	 }

int main(){
	int p, pid, salida;
	char caracteres[NUM_PROCESOS] = {'A','B','C'};
	for (p = 0; p < NUM_PROCESOS; p++){
		pid = fork();
		if(pid == -1 ){
			printf("Error al crear al hijo\n");
			}
		else if (pid == 0){
			imprimir(caracteres[p]);
			}
		}
	for (p = 0; p < NUM_PROCESOS; p++){
		wait(&salida);
		printf("Proceso %d terminado.\n", pid);
		}
	
	}
