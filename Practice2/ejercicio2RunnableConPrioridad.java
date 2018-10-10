public class ejercicio2RunnableConPrioridad implements Runnable{

	String palabra;

	ejercicio2RunnableConPrioridad(String palabraNueva){palabra = palabraNueva;}
	
	public void run(){
		for(int i = 0; i < 10; i++){
			System.out.println(palabra);
		}
	}

	
	
	public static void main(String[] args){
		String[] palabras = {"Hola","Adiós"};
		Thread hilo;
		
		for(int i = 0; i < 2; i++){
			ejercicio2RunnableConPrioridad a = new ejercicio2RunnableConPrioridad(palabras[i]);
			
			hilo = new Thread(a);
			if (i==1) {

				hilo.setPriority(1);
			}
			hilo.start();
		}
	
	}



}
