import java.lang.Math;// For random

public class Dekker extends Thread {

    static int I = 0;
    static volatile int C[] ={0,0};
    //me
    static volatile int id1;
    //other thread
    static volatile int id2;
    static volatile int turn = 0;

    public void run(){
        try{
            for (int i = 0; i < 100; i ++ ){
                //Entry protocol
                C[id1] = 1;
                while(C[id2] == 1){
                    if (turn == C[id2]){
                        C[id1] = 0;
                        while(turn != C[id1]);
                        C[id1] = 1;
                    }
                }
                //critical section
                sleep((long)(100*Math.random()));
                I = I + 1;
                System.out.println("En hilo" + C[id1] + ", I = "+ I);
                //Exit protocol
                C[id1] = 0;
                turn = C[id2];
            }
        }catch(InterruptedException e){return;}
    }

    Dekker(int id){

        this.id1 = id;
        this.id2 = (id == 1)? 0:1;

    }

    public static void main(String args[]){

        Thread thr1 = new Dekker(0);
        Thread thr2 = new Dekker(1);
        thr1.start();
        thr2.start();
    }
}