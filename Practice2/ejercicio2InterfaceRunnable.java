/* Debemos implementar en Java un hilo que lance la impresión de una palabra 10 veces.
* En el main crearemos dos hilos de ese tipo inicializados con dos palabras diferentes y
* los lanzaremos para que se ejecuten a la vez
*/
public class ejercicio2InterfaceRunnable implements Runnable{

	String palabra;

	ejercicio2InterfaceRunnable(String palabraNueva){palabra = palabraNueva;}
	
	public void run(){
		for(int i = 0; i < 10; i++){
			System.out.println(palabra);
		}
	}

	
	
	public static void main(String[] args){
		String[] palabras = {"Hola","Adiós"};
		Thread hilo;
		
		for(int i = 0; i < 2; i++){
			ejercicio2InterfaceRunnable a = new ejercicio2InterfaceRunnable(palabras[i]);
			hilo = new Thread(a);
			hilo.start();
		}
	
	}



}

//http://tutorials.jenkov.com/java-concurrency/creating-and-starting-threads.html
