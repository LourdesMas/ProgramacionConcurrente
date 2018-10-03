/** $hilos
  Compilacion: cc -o hilos hilos.c lpthread
 **/
 
 #include <pthread.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 
 #define NUM_HILOS 3
 int I = 0;
 
 struct valor{
	 int veces;
	 char id;
	 
	 };
 
 void *codigo_del_hilo(void *v){
	 int i;
	 struct valor vaux = *(struct valor*)v;
	 for (i = 0; i < vaux.veces; i++){
		 printf("%c",vaux.id);
		 }
	 
	 pthread_exit(v);
	 }
	 
 int main(){
	 int h;
	 pthread_t hilos[NUM_HILOS];
	 
	 struct valor v[NUM_HILOS] = {{50,'A'},{100,'B'},{150,'C'}};
	 /*v[0].veces = 50;
	 v[0].id = 'A';
	 v[1].veces = 100;
	 v[1].id = 'B';
	 v[2].veces = 150;
	 v[2].id = 'C';*/
	 
	 int error;
	 struct valor *salida;
	 
	 for (h = 0; h <NUM_HILOS; h++){
		 error = pthread_create(&hilos[h],NULL, codigo_del_hilo, &v[h]);
		 if(error){
			 fprintf(stderr, "Error: %d: %s\n",error,strerror (error));
			 exit(-1);
			 
			 }
		 }
	 for (h = 0; h < NUM_HILOS; h++){
		 error = pthread_join(hilos[h],(void**)&salida);
		 if(error){
			 fprintf(stderr, "Error: %d: %s\n", error, strerror(error));
			 }
		 else{
			 printf("\nHilo %c terminado", (char) (*salida).id);
			 }
		 }
	 
	 }
