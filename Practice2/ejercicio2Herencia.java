/* Debemos implementar en Java un hilo que lance la impresión de una palabra 10 veces.
* En el main crearemos dos hilos de ese tipo inicializados con dos palabras diferentes y
* los lanzaremos para que se ejecuten a la vez
*/

public class ejercicio2Herencia extends Thread {


	String palabra;
	Asignaciones(String palabraNueva){palabra = palabraNueva;}

	public void run(){
		for (int i = 0; i < 10; i++){
			System.out.println(palabra);
		}
	}

	public static void main(String[] args){
		String palabras[] = {"Hola","Adiós"};
		Thread hilo;
		for (int i = 0; i < 2; i++){
			hilo = new Thread(new Asignaciones(palabra[i]));
			hilo.start();
		}
		
	}
}
